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

void drawStartingScreen() {  //Giriþ ekraný 
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

void drawInstruction() {  // Talimatlar kýsmý 
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
        gameStarted = true; // enter a basýnca oyun baþlayacak
        //lives = 4;
        //score = 0;
    }
    else if (key == 27) { // esc ye basýnca oyundan çýkacak
        exit(0);
    } 
    else if (key == 105) { // i harfine basýnca talimatlar bölümü açýlacak
        drawInstruction();
    }
    else if (key == 107) {  //k harfine basýnca giriþ ekranýna geri dönecek.
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
    case GLUT_KEY_RIGHT:  // saða gitme
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
            if (brick[i][j] == 1) {   //tuðlayý çizdirecek.
                GLfloat x = j * (brickWidth + 10) + 75;  // tuðlalarýn geniþliðine boþluk ekliyor. Tuðlalarýn dikey ve yatay olarak belirli bir boþluðu (10 ve 5) var
                GLfloat y = i * (brickHeight + 5) + 350; // tuðlalarýn yüksekliðine boþluk ekliyor
                glColor4f(1.0f, 0.0f, 1.0f, 0.0f);
                glRectf(x, y, x + brickWidth, y + brickHeight); // tuðlalarýn konumlarýný kullanarak çizdirir.
            }
        }
    }
}
void initBricks() { //oyun baþladýðýnda tüm bricklerin var oldugunu gösterir.
    for (int i = 0; i < numBrickRows; i++) {
        for (int j = 0; j < numBrickCols; j++) {
            brick[i][j] = 1; // tuðla var
        }
    }
}

// Ball ve Brick arasýndaki çarpýþmayý kontrol eden fonksiyon 
//
void checkCollision() {
    for (int i = 0; i < numBrickRows; i++) { //Bricklerin tüm satýr ve sütunlarýný geziyor.
        for (int j = 0; j < numBrickCols; j++) {
            if (brick[i][j] == 1) {
                GLfloat x = j * (brickWidth + 10) + 75; //her bir brick in konumunu hesaplýyor.
                GLfloat y = i * (brickHeight + 5) + 350;
                if (ballX + ballRadius >= x && ballX - ballRadius <= x + brickWidth 
                    && ballY + ballRadius >= y && ballY - ballRadius <= y + brickHeight) {
                    brick[i][j] = 0; //çarpýþan brick yok edilir. 
                    ballDirY = -ballDirY; //Topun yönü tersine çevrilir.Yani yukarý hareket ediyorsa aþaðý hareket edecek þekilde ayarlanýr.
                    score += 100; // scora her bir tuðla için 100 puan eklenir
                    if (score == numBrickRows * numBrickCols) { // eðer tüm tuðlalar yok edilirse 
                        gameOver = true; // Game over degiskeni true olarak ayarlanýr.

                    }
                }
            }
        }
    }
}

// Ball ve Paddle arasýndaki çarpýþmayý kontrol eder.
//Eðer top paddle ýn üzerindeyse ve paddle ýn sað ve sol sýnýrlarý arasýndaysa topun yönü tersine çevrilir.
void checkPaddleCollision() {
    if (ballY <= paddleY + paddleHeight && ballX >= paddleX && ballX <= paddleX + paddleWidth) {
        ballDirY = -ballDirY; //topun yönü tersine çevrilir.
    }
}


void over() {  // Bitiþ ekraný 
    glClear(GL_COLOR_BUFFER_BIT);
    if (gameOver) { // oyun sonlandýðý zaman Game Over! yazýsý çýkacak
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

void displayScoreAndLives() {  // oyun oynanýrken üst tarafta yer alan Score ve Lives (bricklerin üstünde sað ve solda yer alacak.)
    glColor3f(2.0f, 0.5f, 1.0f);
    glRasterPos2f(30, 570);
    std::string scoreText = "Score: " + std::to_string(score);
    for (int i = 0; i < scoreText.length(); i++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, scoreText[i]);
    }

    glRasterPos2f(710, 570);  // lives ýn konumunu veriyor.
    std::string livesText = "Lives: " + std::to_string(lives);
    for (int i = 0; i < livesText.length(); i++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, livesText[i]);
    }
}

void timer(int value) { // timer fonksiyonu oyun zamanlama iþlemini kontrol etmek için vardýr.
    // oyun döngüsünü sürekli olarak güncellemek için kullanýlan zamanlayýcý fonksiyondur.
    static int prevTime = glutGet(GLUT_ELAPSED_TIME);
    int currTime = glutGet(GLUT_ELAPSED_TIME);
    int deltaTime = currTime - prevTime;
    /*updateBall();*/
    displayScoreAndLives();
    // Pass deltaTime to the update function
    prevTime = currTime;

    glutTimerFunc(10, timer, 0); // Call timer function again after 10ms ( 10 milisaniye sonra çaðýrýr)
}

void updateBall() { //Bu fonksiyon topun konumunu güncellemek ve topun hareket ettiði yöne göre çarpýþmalarý kontrol eder.

    ballX += ballDirX * ballSpeed; //topun mevcut konumunu tutar ve balldirX topun harelet yönünü belirler.
    // ballSpeed ise topun hýzýný arttýrýr.
    ballY += ballDirY * ballSpeed;

    glutPostRedisplay(); // bu fonksiyon ekraný yeniden çizer

    if (ballX + ballRadius >= WIDTH || ballX - ballRadius <= 0) { //Ball sað ve sol sýnýrlara çarpýp çarpmadýgýný check eder.
        ballDirX = -ballDirX; //eðer top oyun alanýný saðýna ve solundaki sýnýra dokunursa balldirX tersine çevrilir
        // ve top yön deðiþtirir.
    }
    if (ballY + ballRadius >= HEIGHT || ballY - ballRadius <= 0) { // topun yukarý ve aþaðý sýnýrlara çarpýp çarpmadýgý kontrol edilir.
        //eðer üst sýnýra dokunursa topun yönü ters çevrilir.
        ballDirY = -ballDirY;
    }



    checkCollision(); //Topun çarpýþma kontrolünü yapmak için bu fonksiyonu çaðýrýr.
    checkPaddleCollision();//Topun çarpýþma kontrolünü yapmak için bu fonksiyonu çaðýrýr.



    if (ballY - ballRadius <= 0) { // eðer top alt zemine dokunursa

        lives--; // caný 1 azalacak.
        if (lives <= 0) { // eðer caný 0 a eþit olursa
            gameOver = true; // Oyun sonlandýrma deðiþkeni true ya dönecek.
            glutTimerFunc(100, timer, 0);


            over(); 
            // oyunu durduracak ve oyun sonucunu ekrana yazdýracak.
            time_t endTime = time(NULL) + 5; // 5 saniye sonra
            while (time(NULL) < endTime) {}
            exit(0); //program sonlandýrýlýr.
        }
        else { // oyuncunun hala caný kaldýysa ;
            ballX = WIDTH / 2; // top baþlangýç konumuna geri döner.
            ballY = HEIGHT - 500;
            paddleX = (WIDTH - paddleWidth) / 2; // paddle baþlangýç konumuna geri döner.
            paddleY = HEIGHT - paddleHeight - 550;
            ballDirY = -1.0;
        }
    }
    glutTimerFunc(20, timer, 0);
}


void displayAndUpdate() {
    glClear(GL_COLOR_BUFFER_BIT); // ekran temizlenir.
    if (!gameStarted) {// oyun baþlamadýysa start screen çaðýrýlýr
        drawStartingScreen();
        //drawInstruction();
    }
    if (gameOver) { // oyun bittiyse over () çaðýrýlarak oyun bitti ekraný çizer.
        over();
    }
    glutSwapBuffers(); //çizim sonuçlarý ekran üzerinde görüntülenir.

}


void init() {
    glClearColor(0.0, 0.0, 0.0, 1.0); //ekran arkaplan rengi. (siyah)
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, WIDTH, 0, HEIGHT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    GLfloat vertices[] = { 0, 0, brickWidth, 0, brickWidth, brickHeight, 0, brickHeight };
    glVertexPointer(2, GL_FLOAT, 0, vertices); //belirtilen brickWidth ve brickHeight ölçülerine sahip bir dikdörtgen oluþturur.


}
void display() {
    //glClear(GL_COLOR_BUFFER_BIT);
    displayAndUpdate();

    if (gameStarted) { // eðer oyun baþlamýþsa;
        glClear(GL_COLOR_BUFFER_BIT);
        drawBall(); // top çizdirilir.
        drawPaddle(); // paddle çizdirilir.
        drawBricks(); // brick çizdirilir.
        displayScoreAndLives(); //score ve lives sayýsý ekrana yazdýrýlýr.
        updateBall(); //topun hareketi güncellenir.
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

    initBricks(); //brickler tanýmlanýyor.
    glutDisplayFunc(display); //oyun ekranýný gösteren display fonk tanýmlanýyor.
    glutKeyboardFunc(keyboard); //keyboard() tanýmlanýyor.
    glutSpecialFunc(specialKeys);
    glutTimerFunc(100, timer, 0);
    init();
    glutMainLoop();
    return 0;
}