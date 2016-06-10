#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <stdint.h>
#include <fstream>
#include <cstring>
#include "iqmloader.hpp"
#include "../error.hpp"
using namespace std;

#define IQM_ERROR "IQM Loader Error"
#define MAX_FILE_SIZE (16<<20)

static glm::mat4 makeBoneMat(glm::quat rot, glm::vec3 trans, glm::vec3 scale) {
  glm::mat3 quatMat = glm::inverse(glm::mat3_cast(glm::normalize(rot)));
  quatMat[0] *= scale;
  quatMat[1] *= scale;
  quatMat[2] *= scale;

  glm::mat4 out = glm::mat4(quatMat);
  out[0].w = trans.x;
  out[1].w = trans.y;
  out[2].w = trans.z;
  out[3].w = 1.0f;

  return out;
}

static bool loadIQMMesh(const string &filename, iqmheader header, uint8_t *buf, IQMModelData &data) {
	iqmvertexarray *iva = (iqmvertexarray *)&buf[header.ofs_vertexarrays];
	float *inposition = NULL, *intexcoord = NULL, *innormal=NULL, *intangent=NULL;
	unsigned char *inblendindex=NULL, *inblendweight=NULL;
	const char *str = header.ofs_text ? (char *)&buf[header.ofs_text] : "";

	for(int i = 0;i < (int)header.num_vertexarrays; i++) {
		iqmvertexarray &va = iva[i];
		switch(va.type) {
			case IQM_POSITION:
				if(va.format != IQM_FLOAT || va.size != 3){
					return false; //Bad vertex position
				}
				inposition = (float *)&buf[va.offset];
				break;
			case IQM_TEXCOORD:
				if(va.format != IQM_FLOAT || va.size !=2){
					return false; //Bad texture coords
				}
				intexcoord = (float *)&buf[va.offset];
				break;
			case IQM_NORMAL:
				if(va.format != IQM_FLOAT || va.size != 3){
					return false; //Bad normal
				}
				innormal = (float*)&buf[va.offset];
				break;
			case IQM_TANGENT:
				if(va.format != IQM_FLOAT || va.size != 4){
					return false; //Bad tangent
				}
				intangent = (float*)&buf[va.offset];
				break;
			case IQM_BLENDWEIGHTS:
				if(va.format != IQM_UBYTE || va.size != 4){
					return false; //Bad blendweight
				}
				inblendweight = (unsigned char*)&buf[va.offset];
				break;
			case IQM_BLENDINDEXES:
				if(va.format != IQM_UBYTE || va.size != 4){
					return false; //Bad indexes
				}
				inblendindex = (unsigned char*)&buf[va.offset];
				break;
		}
	}

	iqmmesh *meshes = (iqmmesh*)&buf[header.ofs_meshes];

  for(int i = 0; i < (int)header.num_meshes; i++) {
    iqmmesh &m = meshes[i];
    
    data.meshes.push_back(m);
    data.textures.push_back(&str[m.material]);
  }

	//load joints
	iqmjoint *joints = (iqmjoint*)&buf[header.ofs_joints];
	data.baseFrame.resize(header.num_joints);
	data.inverseBaseFrame.resize(header.num_joints);
	for(int i=0;i<(int)header.num_joints;i++){
		iqmjoint &j = joints[i];
		data.joints.push_back(j);

		glm::quat q;
		q.x = j.rotate[0];
		q.y = j.rotate[1];
		q.z = j.rotate[2];
		q.w = j.rotate[3];

        glm::vec3 scalevec = glm::vec3(j.scale[0],j.scale[1],j.scale[2]);
        glm::vec3 trans(j.translate[0],j.translate[1],j.translate[2]);

		data.baseFrame[i] = makeBoneMat(q,trans,scalevec);
		data.inverseBaseFrame[i] = glm::inverse(data.baseFrame[i]);

		//If the parent attribute is less than zero, it is a root bone; otherwise it is not
		if(j.parent >= 0){
			data.baseFrame[i] = data.baseFrame[i] * data.baseFrame[j.parent];
			data.inverseBaseFrame[i] =  data.inverseBaseFrame[j.parent] * data.inverseBaseFrame[i];
		}
	}


	//Load triangles;
	iqmtriangle *tris = (iqmtriangle *)&buf[header.ofs_triangles];
	for(size_t i = 0; i < header.num_triangles; i++){
		int t1=tris[i].vertex[0], t3=tris[i].vertex[2];
		tris[i].vertex[0] = t3;
		tris[i].vertex[2] = t1;
	}
	data.triangles.resize(header.num_triangles);
	memcpy(&data.triangles[0],tris,header.num_triangles*sizeof(iqmtriangle));
		
	for(int i=0;i<(int)header.num_vertexes;i++){
		Vertex v;
		if(inposition)
			memcpy(v.position, &inposition[i*3], sizeof(v.position));
		if(intexcoord)
			memcpy(v.texcoord, &intexcoord[i*2], sizeof(v.texcoord));
		if(intangent)
			memcpy(v.tangent, &intangent[i*4],sizeof(v.tangent));
		if(innormal)
			memcpy(v.normal, &innormal[i*3], sizeof(v.normal));
		if(inblendindex)
			memcpy(v.blendindex, &inblendindex[i*4],sizeof(v.blendindex));
		if(inblendweight)
			memcpy(v.blendweight, &inblendweight[i*4],sizeof(v.blendweight));

		data.verticies.push_back(v);
	}

	return true;
}

static bool loadIQMAnim(const string &filename, iqmheader header, uint8_t *buf, IQMModelData &data) {
	if(header.num_poses != header.num_joints)
		return false;

	const char *str = header.ofs_text ? (char*)&buf[header.ofs_text] : "";
	iqmanim *anims = (iqmanim*)&buf[header.ofs_anims];
	iqmpose *poses = (iqmpose*)&buf[header.ofs_poses];

	vector<glm::mat4> frames(header.num_frames * header.num_poses);
	unsigned short *framedata = (uint16_t*)&buf[header.ofs_frames];

	for(int i=0; i < (int)header.num_frames;i++){
		for(int j=0; j < (int)header.num_poses;j++){
			iqmpose &p = poses[j];
			glm::quat rotation; //Rotation quaternion
            glm::vec3 trans, scale; //Translation and scaling vectors, respectively

            //Load data from buffer into variables
            trans.x = p.channeloffset[0]; if(p.mask&0x01) trans.x += *framedata++ * p.channelscale[0];
            trans.y = p.channeloffset[1]; if(p.mask&0x02) trans.y += *framedata++ * p.channelscale[1];
            trans.z = p.channeloffset[2]; if(p.mask&0x04) trans.z += *framedata++ * p.channelscale[2];
            rotation.x = p.channeloffset[3]; if(p.mask&0x08) rotation.x += *framedata++ * p.channelscale[3];
            rotation.y = p.channeloffset[4]; if(p.mask&0x10) rotation.y += *framedata++ * p.channelscale[4];
            rotation.z = p.channeloffset[5]; if(p.mask&0x20) rotation.z += *framedata++ * p.channelscale[5];
            rotation.w = p.channeloffset[6]; if(p.mask&0x40) rotation.w += *framedata++ * p.channelscale[6];
            scale.x = p.channeloffset[7]; if(p.mask&0x80) scale.x += *framedata++ * p.channelscale[7];
            scale.y = p.channeloffset[8]; if(p.mask&0x100) scale.y += *framedata++ * p.channelscale[8];
            scale.z = p.channeloffset[9]; if(p.mask&0x200) scale.z += *framedata++ * p.channelscale[9];

			glm::mat4 m = makeBoneMat(rotation,trans,scale);
			if(p.parent >= 0) 
				frames[i*header.num_poses + j] = data.inverseBaseFrame[j] * (m * data.baseFrame[p.parent]);
			else
				frames[i*header.num_poses+j] = data.inverseBaseFrame[j] * m;
		}

	}

	data.numFrames = header.num_frames-1;

	data.frames = frames;
	for(int i=0;i<(int)header.num_anims;i++){
		iqmanim &a = anims[i];

		data.animations.push_back(a);
		string animName = &str[a.name];
		data.animationNames.push_back(animName);
	}

	return true;
}

bool loadIQM(const string &filename, IQMModelData &data) {
  ifstream file;
  iqmheader header;
  file.open(filename.c_str(), ios::binary);

  if(file.is_open()) {
    file.read((char*)&header, sizeof(iqmheader));

    /* Check model header to make sure this is a valid IQM file */
    if(strcmp(header.magic, IQM_MAGIC) != 0 || header.version != IQM_VERSION) {
      throw Error(IQM_ERROR, "Invalid IQM header: "+filename);
    }

    /* We don't want to load a file that is too large */
    if(header.filesize > MAX_FILE_SIZE) {
      throw Error(IQM_ERROR, "IQM file: "+filename+" file is too large!");
    }

    /* Load the entire file into memory */
    uint8_t *buf = new uint8_t[header.filesize];
    file.read((char*)buf+sizeof(header), header.filesize-sizeof(header));
    file.close();

    loadIQMMesh(filename, header, buf, data);
    loadIQMAnim(filename, header, buf, data);

    /* Clean up buffer we used */
    delete[] buf;

    return true;
  } else {
    throw Error(IQM_ERROR, "IQM file does not exist: "+filename);
  }
}
