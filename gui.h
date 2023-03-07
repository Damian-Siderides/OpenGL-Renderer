#pragma once

// #include "imgui.h"
// #include <stdio.h>

// #include <stdio.h>
// // #include "object.cpp"
#include "object.hpp"
#include <vector>
#include "imgui.h"

void MainMenuBar();

void BootyBay2();

void BootyBay();

// extern bool show_Cabinet_Menu;

void ObjectMenu2();

// extern bool show_Cabinet_List;


void ObjectList(std::vector<Object> *list);

// extern bool show_Object_Menu;

void ObjectMenu();

// int test50; remove this line cause it breaks compilation?

void MainMenuBar();
