include ../AppName

#PKG_CONFIG_PATH=/usr/local/lib/pkgconfig
WXTREE=~/Development/portmedia/wxWidgets-3.1.0/wxmono
WXCONFIG=$(WXTREE)/wx-config
LD_LIBRARY_PATH=$(WXTREE)/lib
#:/usr/local/lib

CXX=`$(WXCONFIG) --cxx`
CXXFLAGS=`$(WXCONFIG) --cxxflags` -I../../rtmidi -I/usr/local/include
LD=`$(WXCONFIG) --ld`
LIBS=`$(WXCONFIG) --libs --gl-libs`

CXX_FILES=$(notdir $(wildcard ../src/*.cpp))
OBJECTS=$(CXX_FILES:.cpp=.o)

VPATH=../src
CONTENTS=$(APPNAME).app/Contents
TARGET=$(CONTENTS)/MacOS/$(APPNAME)


FRAMEWORKS=\
	-arch x86_64 \
	-framework Carbon \
	-framework CoreAudio \
	-framework CoreFoundation \
	-framework CoreMidi \
	-framework OpenGL \
	-framework AGL \
	-framework System \
	-lglew \
	-lglfw3 \
	-lportaudiocpp \
	-lportaudio \
	-lsndfile \
	-lsamplerate \


#	-lwx_osx_carbonu_gl-3.1 \
#	-lwx_osx_carbonu-3.1

#-lwx_macu_gl-2.8.0.7.0
#-lwx_macu-2.8.0.7.0
#-lfftw3f
#-fopenmp

#$(WXTREE)/lib/libwx_macu_gl-2.8.a



all: $(TARGET)

$(TARGET): RtMidi.o $(OBJECTS)
	$(LD) ../$(TARGET) $(LIBS) RtMidi.o $(OBJECTS) $(FRAMEWORKS)

RtMidi.o:
	$(CXX) $(CXXFLAGS) -c -D__MACOSX_CORE__ $(VPATH)/../../rtmidi/RtMidi.cpp -o RtMidi.o

.cpp.o:
	$(CXX) $(CXXFLAGS) -c $< -o $@

bundle:
	cd .. ; echo APPL????>"$(CONTENTS)/PkgInfo"
	cd .. ; dylibbundler -of -od -b \
		-x "$(TARGET)" \
		-d "$(CONTENTS)/Frameworks" \
		-p @executable_path/../Frameworks

run:
	open -W "../$(APPNAME).app"

clean:
	rm -f ../$(TARGET) *.o

