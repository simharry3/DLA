#include "visualizer.h"


float angle = 1.0f;
float red = 1.0f;
float green = 1.0f;
float blue = 1.0f;
float lx=0.0f,lz=-1.0f;
float x=300.0f,y=300.0f,z=300.0f;
float xc=50.0f, yc=50.0f, zc=50.0f;
float particleRadius = 1.0f;
list<vector<int> > particleList;

string renderMode = "solid";

void readInputFile(char* filename){
    
    int numParticles;
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


void drawParticle(int type){
    if(type == 0){
        glColor3f(1.0f, 0.0f, 0.0f);
    }
    else{
        glColor3f(0.0f, 1.0f, 0.0f);
    }
    if(renderMode == "solid"){
        glColor3f(1.0f, 1.0f, 1.0f);
        glutSolidSphere(particleRadius, 10, 10);
    }
    else{
        glutWireSphere(particleRadius, 10, 10);
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
    readInputFile((char*)"output.dat");
    glMatrixMode(GL_MODELVIEW);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();   
    gluLookAt(  x, y, z,
                0.0f, 0.0f, 0.0f,
                0.0f, 1.0f, 0.0f);
    glRotatef(angle, 0.0f, 1.0f, 0.0f);     
    for(list<vector<int> >::iterator i = particleList.begin(); i != particleList.end(); ++i){
        glPushMatrix();
        glTranslatef(((*i)[0]-xc)*particleRadius*2 + particleRadius, ((*i)[1]-yc)*particleRadius*2 + particleRadius, ((*i)[2]-zc)*particleRadius*2 + particleRadius);
        drawParticle((*i)[3]);
        glPopMatrix();   
    }

    angle -= 0.5f;

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
            if(renderMode == "wireframe"){
                renderMode = "solid";
            }
            else{
                renderMode = "wireframe";
            
            }
            break;
    }

}

void printParticles(){
    for(list<vector<int> >::iterator i = particleList.begin(); i != particleList.end(); ++i){
       printf("%d %d %d\n", (*i)[0], (*i)[1], (*i)[2]); 
    }
}

int runVisualizer(void* input){
    tuple<int, char**> args = (tuple<int, char**>)input;
    
    glutInit(args[0], args[1]);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(900, 900);
    glutCreateWindow("GL WINDOW");

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glutDisplayFunc(renderScene);
    glutReshapeFunc(changeSize);
    glutIdleFunc(renderScene);
    glutKeyboardFunc(processNormalKeys);
    glutSpecialFunc(processSpecialKeys);
    // printParticles();

    glutMainLoop();



    return EXIT_SUCCESS;
}