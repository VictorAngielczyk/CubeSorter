/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       C:\Users\vangi                                            */
/*    Created:      Tue Jan 04 2022 <- 3 days before due date btw, 250+ lines */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/

// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// M                    motor         1
// O                    optical       2
// B                    bumper        A
// ---- END VEXCODE CONFIGURED DEVICES ----

////////////////////////
// WHOVER READS THIS, //
// GOOD LUCK, I DON'T //
// PUT MANY COMMENTS  //
// AND C++ IS HELL    //
// PLUS, I DELETED    //
// ANY COMMENTS I     //
// DEEMED USEFUL      //
////////////////////////

#include "vex.h"

using namespace vex;

int g_red = 0;
int g_blue = 0;
int g_green = 0;
int g_other = 0;

enum Dir { R, L };

void clearReset() {

  Brain.Screen.clearScreen();

  Brain.Screen.setCursor(1, 1);

  Brain.Screen.setFont(monoM);
}

void flash() {

  while (true) {

    O.setLight(ledState::on);

    wait(0.1, seconds);

    O.setLight(ledState::off);

    wait(0.1, seconds);
  }
}

void killThreads() {

  while (true) {

    if (B.pressing()) {

      Brain.Screen.clearScreen();

      Brain.Screen.setCursor(1, 1);

      Brain.Screen.setFont(monoXL);

      Brain.Screen.setPenColor(red);

      Brain.Screen.print("EMERGENCY STOP ACTIVATED!!");

      Brain.Screen.drawImageFromFile("kanye.png",0,0);

      std::terminate();
    }

    wait(0.05, seconds);
  }
}

void shake(motor M) {

  M.spinToPosition(-20, degrees, 60);

  M.spinToPosition(20, degrees, 60);
}

void printEnd() {

  Brain.Screen.print("Hopper is empty. Shutting down ...");

  Brain.Screen.newLine();
  
  Brain.Screen.setPenColor(red);
  Brain.Screen.print(g_red);
  Brain.Screen.print(" RED ");
  
  Brain.Screen.setPenColor(red);
  Brain.Screen.print(g_blue);
  Brain.Screen.print(" BLUE ");

  Brain.Screen.setPenColor(red);
  Brain.Screen.print(g_green);
  Brain.Screen.print(" GREEN ");

  Brain.Screen.setPenColor(red);
  Brain.Screen.print(g_other);
  Brain.Screen.print(" OTHER");

  O.setLight(ledState::off);

  std::terminate();
}

bool goodColor(color cur) {

  if (cur == red) {
    return true;
  }

  else if (cur == blue) {
    return true;
  }

  else if (cur == green) {
    return true;
  }

  else if (cur == yellow) {
    return true;
  }

  else {
    return false;
  }
};

Dir *determinePath(color c) {

  static Dir path[2];

  if (c == red) {
    path[0] = R;
    path[1] = R;
    g_red++;
  } else if (c == blue) {
    path[0] = R;
    path[1] = L;
    g_blue++;
  } else if (c == green) {
    path[0] = L;
    path[1] = R;
    g_green++;
  } else if (c == yellow) {
    path[0] = L;
    path[1] = L;
    g_other++;
  }

  return path;
}

void insert(Dir d, motor M) {

  switch (d) {

  case R:
    M.spinToPosition(-50, degrees, 5);
    break;

  case L:
    M.spinToPosition(50, degrees, 5);
    break;
  }

  wait(1, seconds);
}

void filter(Dir d, motor M) {

  switch (d) {

  case R:
    M.spinToPosition(-20, degrees, 20);
    break;

  case L:
    M.spinToPosition(20, degrees, 20);
    break;
  }

  wait(1, seconds);

  M.spinToPosition(0, degrees);
}

int main() {

  vexcodeInit();

  Brain.Screen.print("Touch the screen to begin sorting operation");

  while (!Brain.Screen.pressing()) {

    wait(0.05, seconds);
  }

  clearReset();

  Brain.Screen.print("Crate sorting operation has begun");

  Brain.Screen.newLine();

  Brain.Screen.newLine();

  thread fl = thread(flash);

  thread esc = thread(killThreads);

  //double lastTime = Brain.Timer.value(); implement with exact times

  M.resetRotation();

  O.setLight(ledState::off);

  while (true) {

    int count = 0;

    if ((g_red + g_blue + g_green + g_other) == 8) {

      printEnd();
    }

    while (!goodColor(O.color())) {

      wait(0.1, seconds);

      shake(M);

      if (count == 3) {

        M.spinToPosition(0, degrees);

        printEnd();
      }

      count++;
    }

    M.spinToPosition(0, degrees);

    Brain.Screen.print("Scanning crate ...distributing to storage bin");

    Brain.Screen.newLine();

    Dir *p = determinePath(O.color());

    insert(p[0], M);

    filter(p[1], M);

    wait(2, seconds);
  }
}