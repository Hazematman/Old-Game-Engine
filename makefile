SRCEXT = cpp
CXX = g++
CXXFLAGS := -Wall -std=c++11 -g
LFLAGS =
SOURCEDIR = src
SOURCEDIRS = $(shell find $(SOURCEDIR) -type d)
INCLUDEDIR = $(SOURCEDIR)
BUILDDIR = build
OBJDIR = $(BUILDDIR)/obj
SOURCES = $(shell find $(SOURCEDIR) -name '*.${SRCEXT}')
OBJS = $(addprefix $(OBJDIR)/,$(notdir $(SOURCES:%.${SRCEXT}=%.o)))
BINARY = eigenspace

vpath %.${SRCEXT} $(SOURCEDIRS)


$(OBJDIR)/%.o: %.${SRCEXT}
	@mkdir -p $(OBJDIR)
	$(CXX) $(CXXFLAGS) -I$(INCLUDEDIR) -c $< -o $@

$(BINARY): $(OBJS)
	$(CXX) $(CXXFLAGS) $(LFLAGS) $(OBJS) -o $(BINARY)

.PHONY: clean

clean:
	rm -rf $(BUILDDIR)
	rm -f $(BINARY)
