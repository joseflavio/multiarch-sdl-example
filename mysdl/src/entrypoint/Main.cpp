
#include <vector>
#include <fstream>

#include<math.h> // sqrt

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_opengl.h>

#include <entrypoint/ModelLoader.hpp>


const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_BPP = 32;

// Prototypes
bool opengl_init();
bool init();
void clean_up();

bool opengl_init()
{
    glClearColor(0, 0, 0, 0);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (640.0/480.0), 0.1, 2000.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    if(glGetError() != GL_NO_ERROR)
    {
        return false;
    }

    return true;
}

bool init()
{

    // SDL_Init(SDL_INIT_VIDEO);
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        return false;
    }

    if(SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_OPENGL) == NULL)
    {
        return false;
    }

    if(opengl_init() == false)
    {
        return false;
    }

    SDL_WM_SetCaption("SDL-OpenGL Graphics : Part IV - 3D Shapes", NULL);

    return true;
}

void clean_up()
{
    SDL_Quit();
}

namespace var
{
    Uint32 frame_start_time = 0;
    Uint32 frame_current_time = 0;
    Uint32 frame_count = 0;
}

int main() {

    if(init() == false) {
        std::cerr << "SDL/opengl init failed!\n";
        return 1;
    }

    SDL_Surface* hello = IMG_Load( "anusia.png" ); // SDL_LoadBMP( "bridge.bmp" );
    if(!hello) {
        std::cerr << "File not found!\n";
        return 1;
    }


    const std::string modelFilename = "venusv.obj";
    Model3 aModel;
    if(!ModelLoader::loadFile(aModel, modelFilename, 0.001f)) {
            std::cerr << "Could not read the file! modelFilename="<<modelFilename<<"\n";
            return 1;
    }

    Vertex aLightPos = { 1000.0, -4000.0, 1000.0 };

    std::vector<Dimension> aPowDistances;
    Dimension minPdist, maxPdist;
    FacesUtil::calculatePowDistances(
                aPowDistances, minPdist, maxPdist, aLightPos, aModel.facesCenters);

    std::cout.precision(6);
    std::cout << "vertexes=" << aModel.vertexes.size() << "\n";
    std::cout << "faces=" << aModel.faces.size() << "\n";
    std::cout << "facesCenters=" << aModel.facesCenters.size() << "\n";
    std::cout << "aPowDistances=" << aPowDistances.size() << "\n";

    std::cout << "modelMin=" << aModel.modelMin.toString() << "\n";
    std::cout << "modelMax=" << aModel.modelMax.toString() << "\n";

    std::cout << "minPdist=" << minPdist << "\n";
    std::cout << "maxPdist=" << maxPdist << "\n";

    /****** Main Game Loop ******/
    bool quit = false;
    GLfloat rotation = 0.0f;
    while(quit == false)
    {

        /****** Get Most Current Time ******/
        var::frame_start_time = SDL_GetTicks();

        /****** Draw Rectangle ******/
        glClear(GL_COLOR_BUFFER_BIT);
        glTranslatef(0.0f, 0.0f, -6.0f);
        glRotatef(rotation, 1.0f, 1.0f, 0.0f);

        glBegin(GL_TRIANGLES); // GL_QUADS

        for(size_t i=0; i < aModel.faces.size(); ++i) {

            Dimension colorDistance =
                    (sqrtf(aPowDistances[i]) - sqrtf(minPdist)) /
                    ((sqrtf(maxPdist) - sqrtf(minPdist)) );

            const Face3 cFace = aModel.faces[i];
            const Vertex& v1 = aModel.vertexes[cFace.vi1];
            const Vertex& v2 = aModel.vertexes[cFace.vi2];
            const Vertex& v3 = aModel.vertexes[cFace.vi3];

            glColor4f(colorDistance, colorDistance, colorDistance, 1.0f);
            glVertex3f(v1.x, v1.y, v1.z);
            glVertex3f(v2.x, v2.y, v2.z);
            glVertex3f(v3.x, v3.y, v3.z);
        }


        /* Cube Top */
//        glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
//        glVertex3f(-1.0f, 1.0f, 1.0f);
//        glVertex3f(-1.0f, 1.0f, -1.0f);
//        glVertex3f(1.0f, 1.0f, -1.0f);


        glEnd();
        glLoadIdentity();

        rotation -= 0.5f;

        /****** Check for Key & System input ******/
        SDL_Event event;
        while(SDL_PollEvent(&event))
        {
            /******  Application Quit Event ******/
            if(event.type == SDL_QUIT)
            {
                quit = true;
            }
        }

        /****** Update Screen And Frame Counts ******/
        SDL_GL_SwapBuffers();
        var::frame_count++;
        var::frame_current_time = SDL_GetTicks();

        /****** Frame Rate Handle ******/
        if((var::frame_current_time - var::frame_start_time) < (1000/60))
        {
            var::frame_count = 0;
            SDL_Delay((1000/60) - (var::frame_current_time - var::frame_start_time));
        }
    }
    clean_up();
    return 0;
}
