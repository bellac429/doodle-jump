///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Commenter: Bella Conrad
// Date: 3/6/2024
// Name: hw4\main.cpp
// Description: Edit code to implement scoring and game states.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include <SFML/Graphics.hpp>
#include <time.h>
#include <SFML/Audio.hpp>
#include <string>
#include <iostream>
using namespace sf;

class point {
public:
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// setX method:
// Description: sets the x coordinate of object point to the input integer
//  Inputs: integer, new x coordinate
//  Outputs: None (void).
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    void setX(int xInput) {x = xInput;};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// setY method:
// Description: sets the y coordinate of object point to the input integer
//  Inputs: integer, new y coordinate
//  Outputs: None (void).
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    void setY(int yInput) {y = yInput;};

    int getX() const {return x;};   // getter for x coordinate
    int getY() const {return y;};   // getter for y coordinate
private:
    int x, y;
};

int main()
{
    srand(time(0)); // start seed at 0

    // create game window object called app that's 400 by 533 pixels, titled "Doodle Game!"
    RenderWindow app(VideoMode(400, 533), "Doodle Game!");
    app.setFramerateLimit(60);  // set framerate limit to 60

    Texture t1,t2,t3;   // create three textures and load them from images folder, check if they loaded in correctly
    t1.loadFromFile("images/background.png"); // load background image
    if (!t1.loadFromFile("images/background.png")) {
        return EXIT_FAILURE;
    }
    t2.loadFromFile("images/platform.png"); // load platfrom image
    if (!t2.loadFromFile("images/platform.png")) {
        return EXIT_FAILURE;
    }
    t3.loadFromFile("images/doodle.png"); // load image for character of game
    if (!t3.loadFromFile("images/doodle.png")) {
        return EXIT_FAILURE;
    }

    SoundBuffer buffer; // create sound buffer
    if (!buffer.loadFromFile("sounds/GameOver.ogg")) { // sound from Pixabay.com
        return EXIT_FAILURE;
    }
    Sound sound;    // create sound object
    sound.setBuffer(buffer);   // set object with loaded in sound

    Font font;
    if (!font.loadFromFile("fonts/Typo_Round_Regular_Demo.otf")) {
        return EXIT_FAILURE;
    }
    Text scoreText; // create text object for displaying game score
    scoreText.setFont(font); // select font from Font object
    scoreText.setCharacterSize(28); // set character size in pixels
    scoreText.setFillColor(Color::Black);  // set text color
    scoreText.setStyle(Text::Bold);
    scoreText.setPosition(10,0);

    Text welcome;   // create welcome object to display on welcome screen
    welcome.setFont(font); welcome.setCharacterSize(27); welcome.setFillColor(Color::Black);
    welcome.setString("Welcome to Doodle Jump!");
    welcome.setPosition(45,70);

    Text inst; // create game instructions to be displayed at welcome screen
    inst.setFont(font); inst.setCharacterSize(22); inst.setFillColor(Color::Black);
    inst.setString("Use left and right arrow keys to move");
    inst.setPosition(10,250);
    Text inst2; // create second line of instructions
    inst2.setFont(font); inst2.setCharacterSize(22); inst2.setFillColor(Color::Black);
    inst2.setString("Doodle, don't fall into the void!");
    inst2.setPosition(50,275);
    Text inst3;
    inst3.setFont(font); inst3.setCharacterSize(22); inst3.setFillColor(Color::Black);
    inst3.setString("Press enter to continue");
    inst3.setPosition(80,350);

    Text gameOverText; // create text for displaying game over message
    gameOverText.setFont(font); gameOverText.setCharacterSize(40); gameOverText.setFillColor(Color::Black);
    gameOverText.setString("Game Over!");
    gameOverText.setPosition(80,200);
    bool isGameOver = false; // bool variable that is true when the character dies
    bool isGameStart = true; // bool var that is true when the game starts to display welcome screen

    Text retry; // create retry text object for the user to retry the game after the character dies
    retry.setFont(font); retry.setCharacterSize(28); retry.setFillColor(Color::Black);
    retry.setString("Press r to retry");
    retry.setPosition(105,300);

    int score = 0; // intialize int to hold score, (how many platforms the character jumps on)
    std::vector<int> highScores = {0,0,0,0,0}; // create vector to hold game scores

    // create text displaying "High Scores" before listing top five high scores on game over screen
    Text titleHScores; titleHScores.setFont(font); titleHScores.setCharacterSize(28);
    titleHScores.setFillColor(Color::Black); titleHScores.setStyle(Text::Underlined);
    titleHScores.setString("High Scores");
    titleHScores.setPosition(135,50);

    Text displayScores[5];// create array of text objects that will display top five high scores
    for (int m = 0; m < highScores.size(); m++) {
        displayScores[m].setFont(font); displayScores[m].setCharacterSize(27);
        displayScores[m].setFillColor(Color::Black);
    }
    displayScores[0].setPosition(140,90); // set high scores to be displayed next to each other
    displayScores[1].setPosition(170,90);
    displayScores[2].setPosition(200,90);
    displayScores[3].setPosition(230,90);
    displayScores[4].setPosition(260,90);

    // create three sprites using textures that were loaded
    Sprite sBackground(t1), sPlat(t2), sPers(t3);

    point plat[20]; // create an array of 20 point objects called plat

    for (int i=0;i<10;i++)  // iterate through 10 plat objects
      {
       plat[i].setX(rand()%400);    // set x coordinate of current plat to a random number from 0-399
       plat[i].setY(rand()%533);    // set y coordinate of current plat to a random number form 0-532
      }

	int x=100,y=100,h=200;  // set starting point for character before any keys are pressed
    float dx=0,dy=0;    // dx and dy offset the platforms

    while (app.isOpen())    // run the program as long as the window is open
    {
        // check all the window's events that were triggered since the last iteration of the loop
        Event e;
        while (app.pollEvent(e))    // check event e
        {
            if (e.type == Event::Closed) // if user presses close button: close the window
                app.close();
        }

        sPers.setPosition(x,y);                     // set position of character at start of game

        if (isGameOver == false && isGameStart == false) { // if the game is not over and the welcome screen has played:
            // start Doodle jump game
            if (Keyboard::isKeyPressed(Keyboard::Right)) {
                x+=3;   // move character three pixels to the right in window if right arrow key is pressed
            }
            if (Keyboard::isKeyPressed(Keyboard::Left)) {
                x-=3;   // move character three pixels left in window if left arrow key is pressed
            }

            dy+=0.2;  // dy increments by 0.2 every frame
            y+=dy;  // increment characters y coordinate
            if (y>500) {    // if sprite is at bottom of window (500th pixel):
                sound.play(); // play game over sound if character hits bottom of screen
                isGameOver = true;
                dy=-10;     // move sprite up by 10 pixels
            }

            if (y<h) {                      // if Doodle jumped up past the 200 pixel mark on the screen
                for (int i=0;i<10;i++) {    // iterate though all platform sprites
                    y=h;    // when character is at peak jump height, move platforms down
                    plat[i].setY(plat[i].getY() - dy);         // move platforms down in game window
                    if (plat[i].getY() > 533) {                      // if platform is at bottom of window:
                        plat[i].setY(0); plat[i].setX(rand()%400);
                        // move the platform back to top of window at a random x position
                    }
                }
            }

            for (int i=0;i<10;i++) {
                // x and y coordinates of character mus be in dimensions of platform to be able to jump on platform
                if ((x+50>plat[i].getX()) && (x+20<plat[i].getX()+68) && (y+70>plat[i].getY()) &&
                    (y+70<plat[i].getY()+14) && (dy>0)) {
                    // if the character is on the platform jump up by 10
                    score++;// INCREMENT SCORE
                    dy=-10;
                }
            }

            app.draw(sBackground);              // draw background image
            app.draw(sPers);                    // draw character
            for (int i=0;i<10;i++) {                    // draw all ten platforms on screen by iterating through array
                sPlat.setPosition(plat[i].getX(),plat[i].getY());     // set position of current platform
                app.draw(sPlat);                    // draw platform
            }
            scoreText.setString(std::to_string(score)); // convert score number to a string and set as string to display
            app.draw(scoreText);

        }

        else if (isGameStart == true) { // display welcome screen if the app was opened
            app.clear(Color:: White);
            app.draw(welcome);
            app.draw(inst); // display welcome screen instructions
            app.draw(inst2);
            app.draw(inst3);
            sPers.setPosition(150,130);  // display character on welcome screen
            app.draw(sPers);
            if (Keyboard::isKeyPressed(Keyboard::Enter)) {
                isGameStart = false; // the game has started, bool starts the while loop for in-game displays
            }
        }

        else if (isGameOver == true) {
            if (score >  highScores.at(0)) {// check if current score is a high score
                for (int i = 4; i > 0; i--) {// iterate through other high scores to add current high score
                    highScores.at(i) = highScores.at(i-1);
                }
                highScores.at(0) = score;// add current high score to list of top five high scores
            }
            for (int k = 0; k < highScores.size(); k++) { // change vector of high scores into text objects for display
                displayScores[k].setString(std::to_string(highScores.at(k)));
            }
            app.clear(Color::White);
            app.draw(titleHScores);
            for (int n = 0; n < highScores.size(); n++) {
                app.draw(displayScores[n]);
            }
            app.draw(gameOverText); // display game over screen
            app.draw(retry); // display retry game message


            if (Keyboard::isKeyPressed(Keyboard::R)) {
                score = 0; // reset score when game is over
                for (int i=0;i<10;i++)  // iterate through 10 plat objects
                {
                    plat[i].setX(rand()%400); // set x coordinate of current plat to a random number from 0-399
                    plat[i].setY(rand()%533); // set y coordinate of current plat to a random number form 0-532
                }
                isGameOver = false;
            }
        }

        app.display();  // display drawings and end the current frame
    }
    for (int j = 0; j < highScores.size(); j++) {
        std::cout << highScores.at(j) << " ";
    }

    return 0;
}
