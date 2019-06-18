
bool handleMotivator1(char key) {
  if (key == 'A') {
    primeMotivator(0, 1);
  } else if (key == 'C') {
    primeMotivator(0, 2);
  }
  return false; 
}

bool handleMotivator2(char key) {
  if (key == 'B') {
    primeMotivator(1, 1);
  } else if (key == 'D') {
    primeMotivator(1, 2);
  }
  return false; 
}

bool handleJump(char key) {
  if (key == 'l') {
    if (allPrimed()) { // we have successfully primed all motivators
      if (jumpMode == 1) {
        queueModifiers(_SUPERCRUISE_MOD);
        queueKeystroke(_SUPERCRUISE);
      } else if (jumpMode == 2) {
        queueModifiers(_HYPERSPACE_MOD);
        queueKeystroke(_HYPERSPACE);
      } else {
        queueKeystroke(_JUMP_DISENGAGE);
      }
    } else {
      // Failed jump attempt; reset motivator for next discharge
      resetAllMotivators();
    }
    stabilizeAllMotivators();
    return true;
  }
  return false;
}

bool handle0Throttle(char key) {
  if (key == 'P' || key == 'O') {
    queueModifiers(_ZERO_THROTTLE_MOD);
    queueKeystroke(_ZERO_THROTTLE);
    return true; 
  }
  return false;
}

bool handle50Throttle(char key) {
  if (key == 'N') {
    queueModifiers(_FIVE_ZERO_FORWARD_MOD);
    queueKeystroke(_FIVE_ZERO_FORWARD);
    return true;
  } else if (key == 'M') {
    queueModifiers(_FIVE_ZERO_REVERSE_MOD);
    queueKeystroke(_FIVE_ZERO_REVERSE);
    return true;
  }
  return false;
}

bool handle75Throttle(char key) {
  if (key == 'c') {
    queueModifiers(_SEVEN_FIVE_FORWARD_MOD);
    queueKeystroke(_SEVEN_FIVE_FORWARD);
    return true;
  } else if (key == 'd') {
    queueModifiers(_SEVEN_FIVE_REVERSE_MOD);
    queueKeystroke(_SEVEN_FIVE_REVERSE);
    return true;
  }
  return false;
}

bool handle100Throttle(char key) {
  if (key == 'b') {
    queueModifiers(_ONE_ZERO_ZERO_FORWARD_MOD);
    queueKeystroke(_ONE_ZERO_ZERO_FORWARD);
    return true;
  } else if (key == 'a') {
    queueModifiers(_ONE_ZERO_ZERO_REVERSE_MOD);
    queueKeystroke(_ONE_ZERO_ZERO_REVERSE);
    return true;
  }
  return false;
}

bool handleSafety(char key) {
  if (key == 'k') {
    queueKeystroke(_JUMP_DISENGAGE);
    return true;
  } else if (key == 'j') {
    queueModifiers(_JETTISON_CARGO_MOD);
    queueKeystroke(_JETTISON_CARGO);
    return true;
  } else if (key == 'i') {
    queueModifiers(_SELF_DESTRUCT_MOD);
    queueKeystroke(_SELF_DESTRUCT);
    return true;
  }
  return false;
}

bool handleShipPeripherals(char key) {
  if (key == 'h') {
    queueKeystroke(_TOGGLE_CARGO_SCOOP);
    return true;
  } else if (key == 'g') {
    queueKeystroke(_TOGGLE_LANDING_GEAR);
    return true;
  } else if (key == 'e') {
    queueKeystroke(_TOGGLE_LIGHTS);
    return true;
  } else if (key == 'f') {
    queueKeystroke(_TOGGLE_HARDPOINTS);
    return true;
  }
  return false;
}

bool handleVisualAugmentation(char key) {
  if (key == 'E') {
    queueKeystroke(_TOGGLE_ORBIT_LINES);
    return true;
  } else if (key == 'F') {
    queueModifiers(_TOGGLE_ROTATONAL_CORRECTION_MOD);
    queueKeystroke(_TOGGLE_ROTATONAL_CORRECTION);
    return true;
  } else if (key == 'G') {
    queueModifiers(_TOGGLE_NIGHT_VISION_MOD);
    queueKeystroke(_TOGGLE_NIGHT_VISION);
    return true;
  } else if (key == 'H') {
    queueKeystroke(_SWITCH_HUD_MODE);
    return true;
  }
  return false;
}

bool handleTravelAssist(char key) {
  if (key == 'I') {
    queueModifiers(_OPEN_GALAXY_MAP_MOD);
    queueKeystroke(_OPEN_GALAXY_MAP);
    return true;
  } else if (key == 'J') {
    queueModifiers(_OPEN_SYSTEM_MAP_MOD);
    queueKeystroke(_OPEN_SYSTEM_MAP);
    return true;
  } else if (key == 'K') {
    queueModifiers(_NEXT_SYSTEM_IN_ROUTE_MOD);
    queueKeystroke(_NEXT_SYSTEM_IN_ROUTE);
    return true;
  } else if (key == 'L') {
    queueKeystroke(_DISMISS_RECALL_SHIP);
    return true;
  }
  return false;
}
