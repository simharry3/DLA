#include "visualizer.h"


float angle = 1.0f;
float red = 1.0f;
float green = 1.0f;
float blue = 1.0f;
float lx=0.0f,lz=-1.0f;
float x=300.0f,y=300.0f,z=300.0f;
float xc=30.0f, yc=30.0f, zc=30.0f;
float particleRadius = 1.0f;
list<vector<int> > particleList;

Universe* univ = NULL;

string renderMode = "single";
bool showBoundingBox = false;
bool rotation = true;
bool showActive = true;

void readInputFile(char* filename){
    ifstream dataFile;
    dataFile.open(filename);
    if(!dataFile){
        fprintf(stderr, "UNABLE TO OPEN FILE");
        exit(1);
    }
    int type,xin,yin,zin;
    particleList.clear();
    while(dataFile >> type >> xin >> yin >> zin){
        vector<int> tempVec;
        tempVec.push_back(xin);
        tempVec.push_back(yin);
        tempVec.push_back(zin);
        tempVec.push_back(type);
        particleList.push_back(tempVec);
    }
    dataFile.close();
}

void readUniverseData(){
    particleList.clear();
    for(vector<Particle>::iterator i = univ->aggregators.begin(); i != univ->aggregators.end(); ++i){
        vector<int> tempVec;
        tempVec.push_back(i->pos[0]);
        tempVec.push_back(i->pos[1]);
        tempVec.push_back(i->pos[2]);
        tempVec.push_back(0);
        particleList.push_back(tempVec);
    }
    for(list<Particle>::iterator i = univ->activeParticles.begin(); i != univ->activeParticles.end(); ++i){
        vector<int> tempVec;
        tempVec.push_back(i->pos[0]);
        tempVec.push_back(i->pos[1]);
        tempVec.push_back(i->pos[2]);
        tempVec.push_back(1);
        particleList.push_back(tempVec);
    }
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
    text << "Active Particles: " << univ->numParticles;
    printText(x + 10, y + 10, color, text.str());
    text.str(string());
    text << "Aggregator Particles: " << univ->numAggregators;
    printText(x + 10, y + 35, color, text.str());
}

void drawParticle(int type){
    if(type == 0){
        glColor3f(1.0f, 0.0f, 0.0f);
    }
    else{
        glColor3f(0.0f, 1.0f, 0.0f);
    }
    if(renderMode == "single"){
        glColor3f(1.0f, 1.0f, 1.0f);
        glutSolidSphere(particleRadius, 10, 10);
    }
    else{
        glutSolidSphere(particleRadius, 10, 10);
    }
}

void drawBoundingBox(){
    if(showBoundingBox){
        glColor3f(0.7f, 0.7f, 0.7f);
        glutWireCube((univ->bounds[0] * particleRadius + particleRadius) * 2);
    }
}

void changeSize(int w, int h){
    h = max(1, h);
    float ratio = (1.0 * w)/h;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, w, h);
    gluPerspective(45, ratio, 1, 1000);
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
    glRotatef(angle, 0.0f, 1.0f, 0.0f);
    drawBoundingBox();     
    for(list<vector<int> >::iterator i = particleList.begin(); i != particleList.end(); ++i){
            if(!showActive && (*i)[3] == 1){
                continue;
            }
            glPushMatrix();
            glTranslatef(((*i)[0]-xc)*particleRadius*2 + particleRadius, ((*i)[1]-yc)*particleRadius*2 + particleRadius, ((*i)[2]-zc)*particleRadius*2 + particleRadius);
            drawParticle((*i)[3]);
            glPopMatrix();
    }


    float color[3] = {0.5f, 0.5f, 0.5f};
    printText(10, 10, color, "ESC - Exit   |   F1 - Render Mode   |   F2 - Bounding Box   |   F3 - Rotation"); 
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
            if(renderMode == "dual"){
                renderMode = "single";
            }
            else{
                renderMode = "dual";
            
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
    }

}

void printParticles(){
    for(list<vector<int> >::iterator i = particleList.begin(); i != particleList.end(); ++i){
       printf("%d %d %d\n", (*i)[0], (*i)[1], (*i)[2]); 
    }
}

void* runVisualizer(void* uni){
    univ = (Universe*) uni;
    int argc = 1;
    char* argv[1] = {""};
    glutInit(&argc, argv);

    xc = univ->bounds[0]/2;
    yc = univ->bounds[1]/2;
    zc = univ->bounds[2]/2;

    x = (float)univ->bounds[0] * 4;
    y = (float)univ->bounds[1] * 4;
    z = (float)univ->bounds[2] * 4;

    printf("Visualizer started successfully\n");

    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(900, 900);
    glutCreateWindow("DLA Visualizer");

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glutDisplayFunc(renderScene);
    glutReshapeFunc(changeSize);
    glutIdleFunc(renderScene);
    glutKeyboardFunc(processNormalKeys);
    glutSpecialFunc(processSpecialKeys);
    glutMainLoop();
}
