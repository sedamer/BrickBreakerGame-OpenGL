#include <iostream>
#include <vector>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <string>

using namespace std;


const int WIDTH = 800;
const int HEIGHT = 600;


// Ball properties
GLfloat  ballRadius = 10.0;
GLfloat  ballX = WIDTH / 2;
GLfloat  ballY = HEIGHT - 500;

GLfloat  ballDirX = -1.0;
GLfloat  ballDirY = -1.0;
GLfloat  ballSpeed = 5.0;

// Paddle properties
GLfloat  paddleWidth = 100.0;
GLfloat  paddleHeight = 15.0;
GLfloat  paddleX = (WIDTH - paddleWidth) / 2;
GLfloat  paddleY = HEIGHT - paddleHeight - 550;
GLfloat  paddleSpeed = 5.0;

// Brick properties
const int numBrickRows = 10;
const int numBrickCols = 10;
GLfloat  brickWidth = 60;
GLfloat  brickHeight = 17;

int brick[numBrickRows][numBrickCols];

int score = 0;
int lives = 4;
bool gameStarted = false;
bool gameOver = false;

void drawStartingScreen() {  //Giri� ekran� 
    glClear(GL_COLOR_BUFFER_BIT);
    glColor4f(0.0f, 1.0f, 1.0f, 1.0f);
    glRasterPos2f(200, 400);
    std::string message3 = "BRICK BREAKER GAME";
    for (int i = 0; i < message3.size(); i++) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, message3[i]);
    }
    glColor3f(2.0f, 0.5f, 1.0f);
    glRasterPos2f(220, 300);
    std::string message = "1.Press Enter to Start.Then move to paddle right or left.";
    for (int i = 0; i < message.size(); i++) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, message[i]);
    }

    glColor3f(2.0f, 0.5f, 1.0f);
    glRasterPos2f(220, 280);
    std::string message2 = "2.Press ESC to quit.";
    for (int i = 0; i < message2.size(); i++) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, message2[i]);
    }
    glColor3f(2.0f, 0.5f, 1.0f);
    glRasterPos2f(220, 260);
    std::string message4 = "3.Press i to Instructions. Press k to back here.";
    for (int i = 0; i < message4.size(); i++) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, message4[i]);
    }
    glutSwapBuffers();
}

void drawInstruction() {  // Talimatlar k�sm� 
    glClear(GL_COLOR_BUFFER_BIT);
    glColor4f(0.0f, 1.0f, 1.0f, 1.0f);
    glRasterPos2f(100, 400);
    std::string message5 = "Instructions:";
    for (int i = 0; i < message5.size(); i++) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, message5[i]);
    }
    glRasterPos2f(150, 350);
    std::string message6 = "1.Use left and right arrow keys to move paddle.";
    for (int i = 0; i < message6.size(); i++) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, message6[i]);
    }
    glRasterPos2f(150, 320);
    std::string message9 = "2.Break all the bricks to complete the game.";
    for (int i = 0; i < message9.size(); i++) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, message9[i]);
    }
    glRasterPos2f(150, 290);
    std::string message7 = "3.If the ball falls below the paddle, you lose a life.";
    for (int i = 0; i < message7.size(); i++) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, message7[i]);
    }
    glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y) {
    if (key == 13) {
        gameStarted = true; // enter a bas�nca oyun ba�layacak
        //lives = 4;
        //score = 0;
    }
    else if (key == 27) { // esc ye bas�nca oyundan ��kacak
        exit(0);
    } 
    else if (key == 105) { // i harfine bas�nca talimatlar b�l�m� a��lacak
        drawInstruction();
    }
    else if (key == 107) {  //k harfine bas�nca giri� ekran�na geri d�necek.
        drawStartingScreen();
    }
}
void specialKeys(int key, int x, int y) {
    switch (key) {
    case GLUT_KEY_LEFT:     // sola gitme
        if (paddleX > 0) {
            paddleX -= paddleSpeed;
        }
        break;
    case GLUT_KEY_RIGHT:  // sa�a gitme
        if (paddleX + paddleWidth < WIDTH) {
            paddleX += paddleSpeed;
        }
        break;
    }
    glutPostRedisplay();
}

void drawBall() {

    glColor3f(0.0f, 0.5f, 1.0f);  // ball rengi
    glPushMatrix();
    glTranslatef(ballX, ballY, 0.0f); // ball konumu
    glutWireSphere(ballRadius, 200, 200);
    glPopMatrix();
}

void drawPaddle() {

    glColor4f(0.0f, 1.0f, 1.0f, 1.0f); //paddle rengi
    glPushMatrix();
    glTranslatef(paddleX, paddleY, 0.0f); // ball konumu
    glScalef(paddleWidth, paddleHeight, 1.0f);
    glutSolidCube(1.0);
    glPopMatrix();
}

void drawBricks() {
    for (int i = 0; i < numBrickRows; i++) {
        for (int j = 0; j < numBrickCols; j++) {
            if (brick[i][j] == 1) {   //tu�lay� �izdirecek.
                GLfloat x = j * (brickWidth + 10) + 75;  // tu�lalar�n geni�li�ine bo�luk ekliyor. Tu�lalar�n dikey ve yatay olarak belirli bir bo�lu�u (10 ve 5) var
                GLfloat y = i * (brickHeight + 5) + 350; // tu�lalar�n y�ksekli�ine bo�luk ekliyor
                glColor4f(1.0f, 0.0f, 1.0f, 0.0f);
                glRectf(x, y, x + brickWidth, y + brickHeight); // tu�lalar�n konumlar�n� kullanarak �izdirir.
            }
        }
    }
}
void initBricks() { //oyun ba�lad���nda t�m bricklerin var oldugunu g�sterir.
    for (int i = 0; i < numBrickRows; i++) {
        for (int j = 0; j < numBrickCols; j++) {
            brick[i][j] = 1; // tu�la var
        }
    }
}

// Ball ve Brick aras�ndaki �arp��may� kontrol eden fonksiyon 
//
void checkCollision() {
    for (int i = 0; i < numBrickRows; i++) { //Bricklerin t�m sat�r ve s�tunlar�n� geziyor.
        for (int j = 0; j < numBrickCols; j++) {
            if (brick[i][j] == 1) {
                GLfloat x = j * (brickWidth + 10) + 75; //her bir brick in konumunu hesapl�yor.
                GLfloat y = i * (brickHeight + 5) + 350;
                if (ballX + ballRadius >= x && ballX - ballRadius <= x + brickWidth 
                    && ballY + ballRadius >= y && ballY - ballRadius <= y + brickHeight) {
                    brick[i][j] = 0; //�arp��an brick yok edilir. 
                    ballDirY = -ballDirY; //Topun y�n� tersine �evrilir.Yani yukar� hareket ediyorsa a�a�� hareket edecek �ekilde ayarlan�r.
                    score += 100; // scora her bir tu�la i�in 100 puan eklenir
                    if (score == numBrickRows * numBrickCols) { // e�er t�m tu�lalar yok edilirse 
                        gameOver = true; // Game over degiskeni true olarak ayarlan�r.

                    }
                }
            }
        }
    }
}

// Ball ve Paddle aras�ndaki �arp��may� kontrol eder.
//E�er top paddle �n �zerindeyse ve paddle �n sa� ve sol s�n�rlar� aras�ndaysa topun y�n� tersine �evrilir.
void checkPaddleCollision() {
    if (ballY <= paddleY + paddleHeight && ballX >= paddleX && ballX <= paddleX + paddleWidth) {
        ballDirY = -ballDirY; //topun y�n� tersine �evrilir.
    }
}


void over() {  // Biti� ekran� 
    glClear(GL_COLOR_BUFFER_BIT);
    if (gameOver) { // oyun sonland��� zaman Game Over! yaz�s� ��kacak
        glColor3f(1.0f, 0.0f, 0.0f);
        glRasterPos2f(300, 300);
        std::string message8 = "Game Over!";
        for (int i = 0; i < message8.size(); i++) {
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, message8[i]);

        }
        glColor3f(2.0f, 0.5f, 1.0f); // Score 
        glRasterPos2f(300, 250);
        std::string scoreText = "Score: " + std::to_string(score);
        for (int i = 0; i < scoreText.length(); i++) {
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, scoreText[i]);
        }
        glutSwapBuffers();
    }
}

void displayScoreAndLives() {  // oyun oynan�rken �st tarafta yer alan Score ve Lives (bricklerin �st�nde sa� ve solda yer alacak.)
    glColor3f(2.0f, 0.5f, 1.0f);
    glRasterPos2f(30, 570);
    std::string scoreText = "Score: " + std::to_string(score);
    for (int i = 0; i < scoreText.length(); i++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, scoreText[i]);
    }

    glRasterPos2f(710, 570);  // lives �n konumunu veriyor.
    std::string livesText = "Lives: " + std::to_string(lives);
    for (int i = 0; i < livesText.length(); i++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, livesText[i]);
    }
}

void timer(int value) { // timer fonksiyonu oyun zamanlama i�lemini kontrol etmek i�in vard�r.
    // oyun d�ng�s�n� s�rekli olarak g�ncellemek i�in kullan�lan zamanlay�c� fonksiyondur.
    static int prevTime = glutGet(GLUT_ELAPSED_TIME);
    int currTime = glutGet(GLUT_ELAPSED_TIME);
    int deltaTime = currTime - prevTime;
    /*updateBall();*/
    displayScoreAndLives();
    // Pass deltaTime to the update function
    prevTime = currTime;

    glutTimerFunc(10, timer, 0); // Call timer function again after 10ms ( 10 milisaniye sonra �a��r�r)
}

void updateBall() { //Bu fonksiyon topun konumunu g�ncellemek ve topun hareket etti�i y�ne g�re �arp��malar� kontrol eder.

    ballX += ballDirX * ballSpeed; //topun mevcut konumunu tutar ve balldirX topun harelet y�n�n� belirler.
    // ballSpeed ise topun h�z�n� artt�r�r.
    ballY += ballDirY * ballSpeed;

    glutPostRedisplay(); // bu fonksiyon ekran� yeniden �izer

    if (ballX + ballRadius >= WIDTH || ballX - ballRadius <= 0) { //Ball sa� ve sol s�n�rlara �arp�p �arpmad�g�n� check eder.
        ballDirX = -ballDirX; //e�er top oyun alan�n� sa��na ve solundaki s�n�ra dokunursa balldirX tersine �evrilir
        // ve top y�n de�i�tirir.
    }
    if (ballY + ballRadius >= HEIGHT || ballY - ballRadius <= 0) { // topun yukar� ve a�a�� s�n�rlara �arp�p �arpmad�g� kontrol edilir.
        //e�er �st s�n�ra dokunursa topun y�n� ters �evrilir.
        ballDirY = -ballDirY;
    }



    checkCollision(); //Topun �arp��ma kontrol�n� yapmak i�in bu fonksiyonu �a��r�r.
    checkPaddleCollision();//Topun �arp��ma kontrol�n� yapmak i�in bu fonksiyonu �a��r�r.



    if (ballY - ballRadius <= 0) { // e�er top alt zemine dokunursa

        lives--; // can� 1 azalacak.
        if (lives <= 0) { // e�er can� 0 a e�it olursa
            gameOver = true; // Oyun sonland�rma de�i�keni true ya d�necek.
            glutTimerFunc(100, timer, 0);


            over(); 
            // oyunu durduracak ve oyun sonucunu ekrana yazd�racak.
            time_t endTime = time(NULL) + 5; // 5 saniye sonra
            while (time(NULL) < endTime) {}
            exit(0); //program sonland�r�l�r.
        }
        else { // oyuncunun hala can� kald�ysa ;
            ballX = WIDTH / 2; // top ba�lang�� konumuna geri d�ner.
            ballY = HEIGHT - 500;
            paddleX = (WIDTH - paddleWidth) / 2; // paddle ba�lang�� konumuna geri d�ner.
            paddleY = HEIGHT - paddleHeight - 550;
            ballDirY = -1.0;
        }
    }
    glutTimerFunc(20, timer, 0);
}


void displayAndUpdate() {
    glClear(GL_COLOR_BUFFER_BIT); // ekran temizlenir.
    if (!gameStarted) {// oyun ba�lamad�ysa start screen �a��r�l�r
        drawStartingScreen();
        //drawInstruction();
    }
    if (gameOver) { // oyun bittiyse over () �a��r�larak oyun bitti ekran� �izer.
        over();
    }
    glutSwapBuffers(); //�izim sonu�lar� ekran �zerinde g�r�nt�lenir.

}


void init() {
    glClearColor(0.0, 0.0, 0.0, 1.0); //ekran arkaplan rengi. (siyah)
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, WIDTH, 0, HEIGHT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    GLfloat vertices[] = { 0, 0, brickWidth, 0, brickWidth, brickHeight, 0, brickHeight };
    glVertexPointer(2, GL_FLOAT, 0, vertices); //belirtilen brickWidth ve brickHeight �l��lerine sahip bir dikd�rtgen olu�turur.


}
void display() {
    //glClear(GL_COLOR_BUFFER_BIT);
    displayAndUpdate();

    if (gameStarted) { // e�er oyun ba�lam��sa;
        glClear(GL_COLOR_BUFFER_BIT);
        drawBall(); // top �izdirilir.
        drawPaddle(); // paddle �izdirilir.
        drawBricks(); // brick �izdirilir.
        displayScoreAndLives(); //score ve lives say�s� ekrana yazd�r�l�r.
        updateBall(); //topun hareketi g�ncellenir.
    }

    glutSwapBuffers();
}




int main(int argc, char** argv) {
    glutInit(&argc, argv);
    srand(time(NULL));
    glutInitWindowSize(WIDTH, HEIGHT);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowPosition(250, 150);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glutCreateWindow("Brick Breaker");

    initBricks(); //brickler tan�mlan�yor.
    glutDisplayFunc(display); //oyun ekran�n� g�steren display fonk tan�mlan�yor.
    glutKeyboardFunc(keyboard); //keyboard() tan�mlan�yor.
    glutSpecialFunc(specialKeys);
    glutTimerFunc(100, timer, 0);
    init();
    glutMainLoop();
    return 0;
}