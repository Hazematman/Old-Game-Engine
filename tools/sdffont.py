#!/usr/bin/env python
"""
Signed distance field font generator
"""
import argparse
import sys
import pygame
import math
import json

# Create text array
text = ""
for i in range(ord('!'),ord('~')+1):
    text += chr(i)

numChar = len(text)
charWidth = 9
charHeight = 10

def createBitmapFont(size,padding,font):
    pyFont = pygame.font.SysFont(font, size);
    textures = []
    width = 0
    height = 0
    for c in text:
        texture = pyFont.render(c, False, (255, 255, 255))
        textures.append(texture)
        width = max(width, texture.get_width() + padding*2)
        height = max(height, texture.get_height() + padding*2)

    imageWidth = width*charWidth
    imageHeight = height*charHeight
    image = pygame.Surface((imageWidth, imageHeight))

    for y in range(charHeight):
        for x in range(charWidth):
            image.blit(textures[y*charWidth + x], (x*width+padding,y*height+padding))

    return (image, width, height)

def distance(x1,y1,x2,y2):
    return math.sqrt((x1-x2)**2 + (y1-y2)**2)

def remap(value, oldmin, oldmax, newmin, newmax):
    oldRange = oldmax - oldmin
    newRange = newmax - newmin
    return (((value-oldmin)*newRange)/oldRange)+newmin

def getSDF(bitmap, spread):
    sdf = pygame.Surface((bitmap.get_width(),bitmap.get_height()))
    bitmappx = pygame.PixelArray(bitmap)
    sdfpx = pygame.PixelArray(sdf)
    maxdist = math.sqrt(spread**2 + spread**2)
    for y in range(sdf.get_height()):
        for x in range(sdf.get_width()):
            negate = 1
            if bitmappx[x,y] == bitmap.map_rgb((0,0,0)):
                target = bitmap.map_rgb((255,255,255))
                negate = -1
            else:
                target = bitmap.map_rgb((0,0,0))
            closest = maxdist
            for yy in range(y-int(spread),y+int(spread)):
                for xx in range(x-int(spread),x+int(spread)):
                    if(xx < 0 or xx >= sdf.get_width()):
                        continue
                    if(yy < 0 or yy >= sdf.get_height()):
                        continue
                    if bitmappx[xx,yy] == target:
                        dist = distance(x,y,xx,yy)
                        if dist < closest:
                            closest = dist

            closest = remap(closest*negate, -maxdist, maxdist, 0, 255) 
            sdfpx[x,y] = (closest,closest,closest)
                    

    return sdf

def save(img, name, width, height, pad, spread):
    pygame.image.save(img, name+".png")
    data = {'texture':name+".png", 'charWidth':width, 'charHeight':height,
            'padding':pad, 'spread':spread}
    with open(name+".fnt", "w") as outfile:
        json.dump(data, outfile, indent=4)


def main():
    pygame.font.init()
    parser = argparse.ArgumentParser()
    parser.add_argument('-S','--size', help="character size", default=24, type=int, dest="size")
    parser.add_argument('-r','--spread', help="sdf spread", default=1.0, type=float, dest="spread")
    parser.add_argument('-p','--padding', help="sdf padding", default=1, type=int, dest="padding")
    parser.add_argument('-o','--output', help="output name", type=str, dest="out")
    parser.add_argument("font", type=str)
    args = parser.parse_args()

    (bitmap,width,height) = createBitmapFont(args.size, args.padding, args.font)
    sdf = getSDF(bitmap, args.spread)
    name = args.font
    if args.out:
        name = args.out

    save(sdf, name, width, height, args.padding, args.spread)
    return 0

if __name__ == "__main__":
    sys.exit(main())
