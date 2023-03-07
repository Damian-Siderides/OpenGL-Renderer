#include "imgui.h"
#include <stdio.h>
#include "gui.h"
// #include "object.cpp"
#include "object.hpp"
#include <vector>

#define FIELD_LENGTH (128)

extern std::vector<Object> objects;

void MainMenuBar() {
    
    if (ImGui::BeginMainMenuBar()) {
        
        if (ImGui::BeginMenu("File"))
        {
            //ShowExampleMenuFile();
            if (ImGui::MenuItem("New", "CTRL+N")) {}
            if (ImGui::MenuItem("Open", "CTRL+O")) {}
            if (ImGui::MenuItem("Open Recent")) {}
            ImGui::Separator();
            if (ImGui::MenuItem("Save", "CTRL+S")) {}
            if (ImGui::MenuItem("Save As", "CTRL+Z")) {}
            ImGui::Separator();
            if (ImGui::MenuItem("Delete")) {}
            ImGui::Separator();
            if (ImGui::MenuItem("Settings")) {}
            if (ImGui::MenuItem("Quit", "Alt+F4")) {}
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Edit"))
        {
            if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
            if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
            ImGui::Separator();
            if (ImGui::MenuItem("Cut", "CTRL+X")) {}
            if (ImGui::MenuItem("Copy", "CTRL+C")) {}
            if (ImGui::MenuItem("Paste", "CTRL+V")) {}
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
}

void BootyBay2() {
    ImGui::Begin("Booty Bay 2");
    ImGui::Text("Welcome to Booty Bay");

    // Object *x = new Object();

    // ImGui::Text("X: %f", x->x);
    ImGui::Text("Welcome to Booty Bay 2");
    ImGui::Text("Welcome to Booty Bay");

    ImGui::End();
}

void BootyBay() {

    bool extraWindow = true;

    ImGui::Begin("Booty Bay");
    ImGui::Text("Welcome to Booty Bay");

    // Object *x = new Object();

    double y = 5.0;

    // ImGui::Text("X: %f", x->x);
    ImGui::Text("Welcome to Booty Bay");
    ImGui::Text("Welcome to Booty Bay");
    ImGui::Text("Welcome to Booty Bay Hello there, wasup i ahve been recompiled, hello there");


    // if (ImGui::IsKeyPressed(ImGuiKey_Space)) {
    //     extraWindow = true;
    // }

    if (extraWindow) {
        BootyBay2();
    }

    ImGui::End();

}

extern bool show_Object_Menu2;

void ObjectMenu2() {
    ImGui::Begin("Object Menu", &show_Object_Menu2, 0);
    ImGui::Text("Welcome to the Object Menu.");

    ImGui::Text("Please enjoy your time.");

    // if (ImGui::Button("Thing")) {
    //         printf("You clicked the button!\n");
    // }

    // static int clicked = 0;
    //     if (ImGui::Button("Button"))
    //         clicked++;
    //     if (clicked & 1)
    //     {
    //         ImGui::SameLine();
    //         ImGui::Text("Thanks for clicking me!");
    //     }

    ImGui::End();
}

extern bool show_Object_List;


void ObjectList(std::vector<Object> *list) {
    ImGui::Begin("Object List", &show_Object_List, 0);

    for (unsigned int i = 0; i < list->size(); i++) {

        ImGui::Text("ID is: %d\n", list->at(i).id);
        ImGui::Text("Name is: %s\n", list->at(i).name);
        ImGui::Text("Material is: %s\n", list->at(i).material);

        ImGui::Text("(X, Y, Z) is: (%f, %f, %f)\n", list->at(i).x, list->at(i).y, list->at(i).z);
        ImGui::Text("(I, J, K) is: (%f, %f, %f)\n", list->at(i).i, list->at(i).j, list->at(i).k);
        ImGui::Text("Width: %f, Height: %f, Depth: %f\n", list->at(i).width, list->at(i).height, list->at(i).depth);

        // ImGui::Separator();

        // char name[128] = "Object ";
        
        char name[FIELD_LENGTH] = "Object ";
        char temp[10];
        sprintf(temp, "%d", i + 1);

        strcat(name, temp);

        char temp2[128];

        strcpy(temp2, name);

        char id[128];
        // strcat(name, (char*) i + '0');

        strcat(name, " Name ");
        ImGui::Text("Name:     ");
        ImGui::SameLine();
        ImGui::InputText(name, list->at(i).name, IM_ARRAYSIZE(list->at(i).name));
        strcpy(name, temp2);

        strcat(name, " Material");
        ImGui::Text("Material: ");
        ImGui::SameLine();
        ImGui::InputText(name, list->at(i).material, IM_ARRAYSIZE(list->at(i).material));
        strcpy(name, temp2);

        strcat(name, " Width ");
        ImGui::Text("Width:    ");
        ImGui::SameLine();
        ImGui::InputFloat(name , &(list->at(i).width ));
        strcpy(name, temp2);

        strcat(name, " Height ");
        ImGui::Text("Height:   ");
        ImGui::SameLine();
        ImGui::InputFloat(name, &(list->at(i).height));
        strcpy(name, temp2);

        strcat(name, " Depth ");
        ImGui::Text("Depth:    ");
        ImGui::SameLine();
        ImGui::InputFloat(name , &(list->at(i).depth ));
        strcpy(name, temp2);

        strcat(name, " X ");
        ImGui::Text("X:        ");
        ImGui::SameLine();
        ImGui::InputFloat(name , &(list->at(i).x ));
        strcpy(name, temp2);

        strcat(name, " Y ");
        ImGui::Text("Y:        ");
        ImGui::SameLine();
        ImGui::InputFloat(name, &(list->at(i).y));
        strcpy(name, temp2);

        strcat(name, " Z ");
        ImGui::Text("Z:        ");
        ImGui::SameLine();
        ImGui::InputFloat(name , &(list->at(i).z ));
        strcpy(name, temp2);

        ImGui::Separator();
    }
    ImGui::End();
}

extern bool show_Object_Menu;

void ObjectMenu() {
    ImGui::Begin("Object Menu", &show_Object_Menu, 0);
    if (ImGui::Button("New Object")) {
        // Object* object2 = new Object(500, 500, 500);
        objects.emplace_back(500, 500, 500);
    }
    ImGui::End();
}