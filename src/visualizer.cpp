#include "visualizer.h"
#include "utilities.h"

using namespace std;

float angle = 90.0f;
float x=300.0f,y=300.0f,z=300.0f;
float xc=30.0f, yc=30.0f, zc=30.0f;
float xz = 0.0f, yz = 0.0f, zz = 0.0f;
float ratio = 1.0;
float particleRadius = 1.0f;
float pointSize = 0.2f;
int subdivisions = 3;
list<vector<int> >* particleList;

Universe* univ = NULL;

string renderMode = "fast";
bool showBoundingBox = false;
bool rotation = false;
bool showActive = true;
bool smoothing = false;


void readInputFile(char* filename){
    ifstream dataFile;
    dataFile.open(filename);
    if(!dataFile){
        fprintf(stderr, "UNABLE TO OPEN FILE");
        exit(1);
    }
    int type,xin,yin,zin;
    particleList->clear();
    while(dataFile >> type >> xin >> yin >> zin){
        vector<int> tempVec;
        tempVec.push_back(xin);
        tempVec.push_back(yin);
        tempVec.push_back(zin);
        tempVec.push_back(type);
        particleList->push_back(tempVec);
    }
    dataFile.close();
}

void readUniverseData(){
    delete particleList;
    particleList = univ->generateOutputList();
}

void printText(float x, float y, float* color, string text){
    int winWidth = glutGet(GLUT_WINDOW_WIDTH);
    int winHeight = glutGet(GLUT_WINDOW_HEIGHT);
    // x *= winWidth;
    // y *= winWidth;
    int len = text.length();
    glColor3f(color[0], color[1], color[2]);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0.0, winWidth, 0.0, winHeight);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glRasterPos2i(x, y);

    for(string::iterator i = text.begin(); i != text.end(); ++i){
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *i);
    }

    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
}

void drawStatsBox(float x, float y, float* color){
    int winWidth = glutGet(GLUT_WINDOW_WIDTH);
    int winHeight = glutGet(GLUT_WINDOW_HEIGHT);
    // x *= winWidth;
    // y *= winWidth;

    glColor3f(color[0], color[1], color[2]);
    ostringstream text;
    text << "Active Particles: " << univ->getNumActiveParticles();
    printText(x + 10, y + 10, color, text.str());
    text.str(string());
    text << "Aggregator Particles: " << univ->getNumAggregators();
    printText(x + 10, y + 35, color, text.str());
}

void drawParticle(int type){
    if(type == 0){
        glColor3f(1.0f, 0.0f, 0.0f);
    }
    else{
        glColor3f(0.0f, 1.0f, 0.0f);
    }
    if(renderMode == "fancy"){
        glColor3f(1.0f, 1.0f, 1.0f);
        glutSolidSphere(particleRadius, subdivisions, subdivisions);
    }
    else{
        glPointSize(pointSize);
        glBegin(GL_POINTS);
            glVertex2f(0.0, 0.0);
        glEnd();
        // glutSolidSphere(particleRadius, 10, 10);
    }
}

void drawBoundingBox(){
    if(showBoundingBox){
        glColor3f(0.7f, 0.7f, 0.7f);
        const int* bounds = univ->getBounds();
        glutWireCube((bounds[0] * particleRadius + particleRadius) * 2);
    }
}

void changeSize(int w, int h){
    h = max(1, h);
    ratio = (1.0 * w)/h;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, w, h);
    gluPerspective(45, ratio, .1, 1000);
    glMatrixMode(GL_MODELVIEW);
}

void renderScene(void){
    readUniverseData();
    // readInputFile((char*)"output.dat");
    glMatrixMode(GL_MODELVIEW);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();  
    gluLookAt(  x, y, z,
                0.0f, 0.0f, 0.0f,
                0.0f, 1.0f, 0.0f);
    glTranslatef(xz, yz, zz); 
    glRotatef(angle, 0.0f, 1.0f, 0.0f);
    drawBoundingBox();
    if(renderMode == "fancy"){
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);
    }     
    for(list<vector<int> >::iterator i = particleList->begin(); i != particleList->end(); ++i){
            if(!showActive && (*i)[3] == 1){
                continue;
            }
            glPushMatrix();
            glTranslatef(((*i)[0]-xc)*particleRadius*2 + particleRadius, ((*i)[1]-yc)*particleRadius*2 + particleRadius, ((*i)[2]-zc)*particleRadius*2 + particleRadius);
            drawParticle((*i)[3]);
            glPopMatrix();
    }
    if(renderMode == "fancy"){
        glDisable(GL_LIGHTING);
        glDisable(GL_LIGHT0);
    }


    float color[3] = {0.5f, 0.5f, 0.5f};
    printText(10, 10, color,    "ESC - Exit   |   F1 - Mode   |   F2 - Bounds   |   F3 - Rotation   |   F4 - Active"); 
    drawStatsBox(10, glutGet(GLUT_WINDOW_HEIGHT) - 60 , color);

    if(rotation){
        angle -= 0.5f;
    }

    glutSwapBuffers();
}

void processNormalKeys(unsigned char key, int x, int y){
    if(key == 27){
        exit(0);
    }
}

void processSpecialKeys(int key, int x, int y){
    switch(key){
        case GLUT_KEY_F1:
            if(renderMode == "fast"){
                renderMode = "fancy";
            }
            else{
                renderMode = "fast";
                glDisable(GL_LIGHTING);
                glDisable(GL_LIGHT0);
            
            }
            break;
        case GLUT_KEY_F2:
            showBoundingBox = !showBoundingBox;
            break;
        case GLUT_KEY_F3:
            rotation = !rotation;
            break;
        case GLUT_KEY_F4:
            showActive = !showActive;
            break;
    }

}

void processMouse(int button, int state, int x, int y){
    if(button == 3 || button == 4){
        if(state == GLUT_UP){
            return;
        }
        if(button == 3){
            xz += 10;
            yz += 10;
            zz += 10;
        }
        else{
            xz -= 10;
            yz -= 10;
            zz -= 10;            
        }
    }
}

void printParticles(){
    for(list<vector<int> >::iterator i = particleList->begin(); i != particleList->end(); ++i){
       printf("%d %d %d\n", (*i)[0], (*i)[1], (*i)[2]); 
    }
}


//MAIN VISUALIZER LOOP THAT IS PASSED TO PTHREAD:
void* runVisualizer(void* uni){
    univ = (Universe*) uni;
    int argc = 1;
    char* argv[1] = {""};
    glutInit(&argc, argv);

    const int* bounds = univ->getBounds();
    xc = bounds[0]/2;
    yc = bounds[1]/2;
    zc = bounds[2]/2;

    x = (float)bounds[0] * 4;
    y = (float)bounds[1] * 4;
    z = (float)bounds[2] * 4;


    

    printf("Visualizer started successfully\n");

    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(900, 900);
    glutCreateWindow("DLA Visualizer");


    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_shininess[] = { 50.0 };
    GLfloat light_position[] = { x/2, y/2, z/2, 0.0 };
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glutDisplayFunc(renderScene);
    glutReshapeFunc(changeSize);
    glutIdleFunc(renderScene);
    glutKeyboardFunc(processNormalKeys);
    glutSpecialFunc(processSpecialKeys);
    glutMouseFunc(processMouse);
    glutMainLoop();
}
