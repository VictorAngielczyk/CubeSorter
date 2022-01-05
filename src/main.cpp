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

void checkToKill(int i) {

  if (i >= 100) {

    std::terminate();
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

  return path;
}

void insert(Dir d, motor M) {

  switch (d) {

  case R:
    M.spinToPosition(-45, degrees, 5);
    break;

  case L:
    M.spinToPosition(45, degrees, 5);
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

  while (!Brain.Screen.pressing()) {

    wait(0.05, seconds);
  }

  thread esc = thread(killThreads);

  M.resetRotation();

  O.setLight(ledState::on);

  while (true) {

    int count = 0;

    while (!goodColor(O.color())) {

      wait(0.05, seconds);

      shake(M);

      if (count == 30) {

        std::terminate();
      }

      count++;
    }

    M.spinToPosition(0, degrees);

    Dir *p = determinePath(O.color());

    insert(p[0], M);

    filter(p[1], M);

    wait(7, seconds);
  }
}