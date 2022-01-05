/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       C:\Users\vangi                                            */
/*    Created:      Tue Jan 04 2022                                           */
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

////////
// THINGS TO IMPLEMENT
// ALL TEXT
// MEASURED DEGREES

#include "vex.h"

using namespace vex;



enum Dir { R, L };

void flash() {

  while(true) {

    O.setLight(ledState::on);

    wait(1,seconds);

    O.setLight(ledState::off);

    wait(1,seconds);
  }
}

void killThreads() {

  while (true) {

    if (B.pressing()) {

      Brain.Screen.clearScreen();

      Brain.Screen.setCursor(1, 1);

      Brain.Screen.setFont(monoXL);

      Brain.Screen.print("EMERGENCY STOP ACTIVATED!!"); // MAKE RED

      std::terminate();
    }

    wait(0.05, seconds);
  }
}

void shake(motor M) {

  M.spinToPosition(-20, degrees, 60);

  M.spinToPosition(20, degrees, 60);
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
  }
  else if (c == blue) {
    path[0] = R;
    path[1] = L;
  }
  else if (c == green) {
    path[0] = L;
    path[1] = R;
  }
  else if (c == yellow) {
    path[0] = L;
    path[1] = L;
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

  // M.spinToPosition(0,degrees);
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
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();

  //Brain.Screen.drawImageFromBuffer(KANYE,0,0,40,40);

  while (!Brain.Screen.pressing()) {

    wait(0.05, seconds);
  }

  thread fl = thread(flash);

  thread esc = thread(killThreads);

  M.resetRotation();

  O.setLight(ledState::off);

  while (true) {

    int count = 0;

    while (!goodColor(O.color())) {

      wait(0.05, seconds);

      shake(M);

      if (count == 20) {

        M.spinToPosition(0, degrees);

        std::terminate();
      }

      count++;
    }

    M.spinToPosition(0, degrees);

    Dir *p = determinePath(O.color());

    insert(p[0], M);

    filter(p[1], M);

    wait(2, seconds);
  }
}