#include <iostream>
#include <raylib.h>
#include "rlImGui/rlImGui.h"
#include "imgui/imgui.h"
#include "misc/cpp/imgui_stdlib.h"
#include "imgui/imfilebrowser.h"
#include "definitions.h"
#include "miniz.h"
#include <filesystem>
#include <sys/mount.h>

#include <thread>
#include <atomic>
#include <cmath>

/* URLs */
// Bad Update
std::string b_upd_10 = "https://github.com/grimdoomer/Xbox360BadUpdate/releases/download/v1.0/Xbox360BadUpdate-Retail-USB.zip";
std::string b_upd_11 = "https://github.com/grimdoomer/Xbox360BadUpdate/releases/download/v1.1/Xbox360BadUpdate-Retail-USB-v1.1.zip";
std::string b_upd_12 = "https://github.com/grimdoomer/Xbox360BadUpdate/releases/download/v1.2/Xbox360BadUpdate-Retail-USB-v1.2.zip";
std::string b_upd_13 = "https://github.com/grimdoomer/Xbox360BadUpdate/releases/download/BadUpdate-v1.3/Xbox360BadUpdate-Retail-USB-v1.3.zip";
std::string b_upd_latest = b_upd_13; // :-)
// ABadAvatar
std::string b_avt_b1 = "https://github.com/shutterbug2000/ABadAvatar/releases/download/vPB1.0/ABadAvatar-publicbeta1.0.zip";


// That func is not written by me :-p
std::atomic<bool> is_working(false);
void run_async(const std::string& command, std::atomic<bool>& status_flag) {
    if (status_flag) return; // Prevent double-triggering while already running

    status_flag = true;

    std::thread([command, &status_flag]() {
        std::system(command.c_str());
        status_flag = false; // Automatically resets when finished
    }).detach();
}
// </>

int simple_execute(std::string command) {
    int result = std::system(command.c_str());
    return result;
}

std::vector<std::string> device_list = {"Please scan the devices to generate a valid entry"};
int scan_devices() {
    device_list.clear();
    for (const auto& entry : std::filesystem::directory_iterator("/dev")) {
        std::string name = entry.path().filename().string();
        if (name.rfind("sd", 0) == 0) {
            std::cout << "/dev/" << name << std::endl;
            device_list.push_back("/dev/" + name);
        }
    }
    if (device_list.empty()) {
        device_list.push_back("No USB devices found.");
    }
    return 0;
}

int main() {
    rlImGuiSetup(true);
    ImGui::PushStyleColor(ImGuiCol_PlotHistogram,
        ImVec4(0.2f, 0.8f, 0.2f, 1.0f));

    std::string drive_name = "sdb";
    int payload_type = 0;
    ImGui::FileBrowser FileBrowser;
    std::string game_path = "path/to/game.iso";
    std::string game_folder = "./name_of_the_folder";
    int payload_game = 0;
    int action_selected = 0;
    int badupdate_version = NEWEST;
    int badavatar_version = 1;
    std::string disk_path;
    std::string command;
    std::string ver_SEL;
    int usb_selected = 0;


    FileBrowser.SetTitle("Select .iso game to convert");
    FileBrowser.SetTypeFilters({".iso", ".ISO"});

    SetConfigFlags(FLAG_FULLSCREEN_MODE);

    InitWindow(640, 480, "Xbox 360 Central");

    Image logo = LoadImage("./Xbox_logo.png");
    Texture2D logo_tex = LoadTextureFromImage(logo);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawText("Press 'ESC' to exit.", 10, GetScreenHeight()-50, 20, GREEN);
        DrawTexture(logo_tex, 20, 20, WHITE);


        rlImGuiBegin();

        int show_err_win = 0;
        std::string err_win_error = "";
        if (show_err_win) {
            ImGui::Begin("Error! Something went wrong.");
            ImGui::TextColored(ImVec4(1.0f, 0.1f, 0.1f, 1.0f), err_win_error.c_str());
            if (ImGui::Button("Ok")) show_err_win = 0;
            ImGui::End();
        }


        ImGui::Begin("BadUpdate - Setup");
            ImGui::TextColored(ImVec4(0.1f, 1.0f, 0.1f, 1.0f),
                "%s", "Make a Softmod USB");
            ImGui::SetNextItemWidth(40.0f);
           // ImGui::InputText("Enter the name of your USB drive: (eg. sdb, nvme0, etc.) ",
            //    &drive_name);
            //ImGui::Text("Current drive selected: /dev/%s", drive_name.c_str());
            if (ImGui::Button("Scan USB devices")) scan_devices();
            for (int i = 0; i < device_list.size(); i++) {
                std::string label = device_list[i] + "###/dev/" + std::to_string(i);
                ImGui::RadioButton(label.c_str(), &usb_selected, i);
            }
            ImGui::TextDisabled("Choosen USB: %s, debug num: %d", device_list[usb_selected].c_str(), usb_selected);
            ImGui::RadioButton("BadUpdate", &payload_type, BAD_UPDATE);
            ImGui::RadioButton("ABadAvatar", &payload_type, BAD_AVATAR);
            ImGui::RadioButton("I'll figure it out###1", &payload_type, NONE);
            if (payload_type == BAD_UPDATE) {
                ImGui::Text("Select which game do you want the exploit to work with");
                if (badupdate_version != NEWEST && badupdate_version != 3)
                    ImGui::RadioButton("Tony Hawk American Westland (Free demo)",
                        &payload_game, TONY_HAWK);
                ImGui::RadioButton("Rock Band Blitz (Free demo)", &payload_game,
                    ROCK_BAND);
                ImGui::RadioButton("I'll figure it out###2", &payload_game, NONE);
            }
        if (payload_type != NONE) ImGui::Text("Select payload version");
        if (payload_type == BAD_UPDATE) {
            ImGui::RadioButton("Newest (default)", &badupdate_version, NEWEST);
            ImGui::RadioButton("1.3", &badupdate_version, 3); ImGui::SameLine();
            ImGui::RadioButton("1.2", &badupdate_version, 2); ImGui::SameLine();
            ImGui::RadioButton("1.1", &badupdate_version, 1); ImGui::SameLine();
            ImGui::RadioButton("1.0", &badupdate_version, 0);
        }
        if (payload_type == BAD_AVATAR) {
            ImGui::RadioButton("Beta 1.0", &badavatar_version, 1);
        }
        ImGui::End();
        ImGui::Begin("Convert iso to god - Setup");
            ImGui::TextColored(ImVec4(0.1f, 1.0f, 0.1f, 1.0f), "%s",
                "Convert games for Aurora");
            ImGui::SetNextItemWidth(300.0f);
            ImGui::InputText("Path", &game_path); ImGui::SameLine();
            if (ImGui::Button("Browse...")) FileBrowser.Open();
            FileBrowser.Display();
            if (FileBrowser.HasSelected()) {
                std::string tmp_path = FileBrowser.GetSelected().string();
                game_path = tmp_path;
                FileBrowser.ClearSelected();
            }
            ImGui::TextDisabled("Selected game: %s", game_path.c_str());
            ImGui::SetNextItemWidth(200.0f);
            ImGui::InputText("New folder name", &game_folder);
        ImGui::End();
        ImGui::Begin("Control Panel");
            ImGui::TextColored(ImVec4(0.1f, 1.0f, 0.1f, 1.0f), "%s",
                "Control setups");
            ImGui::Text("Select action:");
            ImGui::RadioButton("Install payload to USB", &action_selected,
                INSTALL_PAYLOAD);
            ImGui::RadioButton("Format .iso to GOD", &action_selected, FORMAT_ISO);
            ImGui::RadioButton("Nothing", &action_selected, NONE);
            ImGui::Text("Confirmation checkout");
            if (action_selected == INSTALL_PAYLOAD) {
                switch (payload_type) {
                    case BAD_UPDATE:
                        if (badupdate_version == NEWEST)
                            ImGui::Text("Install Bad Update (newest) to USB: /dev/%s",
                                drive_name.c_str());
                        else
                            ImGui::Text("Install Bad Update 1.%d to USB: /dev/%s",
                                badupdate_version, drive_name.c_str());
                        switch (payload_game) {
                            case ROCK_BAND:
                                ImGui::Text("Rock Band Blitz will be used to activate BadUpdate."); break;
                            case TONY_HAWK:
                                ImGui::Text("Tony Hawk Pro Skater will be used to activate BadUpdate.");
                                if (badupdate_version == NEWEST || badupdate_version == 3)
                                    ImGui::TextColored(ImVec4(1.0f, 0.1f, 0.1f, 1.0f),
                                        "%s", "TONY HAWK AMERICAN WESTLAND IS NOT SUPPORTED SINCE THE RELEASE OF 1.3");
                                break;
                            default:
                                ImGui::TextDisabled("No game will be used to activate BadUpdate.");
                        }
                        break;
                    case BAD_AVATAR:
                        ImGui::Text("Install Bad Avatar to USB: /dev/%s", drive_name.c_str());
                        break;
                    default:
                        ImGui::TextDisabled("No payload is installed.");
                        break;
                }
            } else if (action_selected == FORMAT_ISO) {
                ImGui::Text("ISO file '%s' will be converted to GOD format to '%s'.", game_path.c_str(), game_folder.c_str());
            } else {
                ImGui::Text("Nothing to see here.");
            }
            switch (action_selected) {
                case FORMAT_ISO:
                    command = "./extract-xiso '" + game_path + "' -d '" + game_folder + "'";
                    break;
                case INSTALL_PAYLOAD:
                    switch (payload_type) {
                        case BAD_UPDATE:
                            switch (badupdate_version) {
                                case NEWEST:
                                    ver_SEL = b_upd_latest;
                                    break;
                                case VER13:
                                    ver_SEL = b_upd_13;
                                    break;
                                case VER12:
                                    ver_SEL = b_upd_12;
                                    break;
                                case VER1:
                                    ver_SEL = b_upd_10;
                                    break;
                                default:
                                    ver_SEL = b_upd_latest;
                                    break;
                            }
                            break;
                    }
                default:
                    command = "sleep 1";
                    break;
            }


            if (ImGui::Button("Exec")) {
              run_async(command,is_working);
            }
        ImGui::End();
        ImGui::Begin("Status");
            if (is_working) {
                ImGui::Text("Working...");
                float progress = (float)(fmod(ImGui::GetTime() / 1.5, 1.0));
                ImGui::ProgressBar(progress, ImVec2(0.0f, 0.0f), " ");
            } else {
                ImGui::TextDisabled("Nothing to see here.");
            }
        ImGui::End();

        ImGui::Begin("About");
        ImGui::TextColored(ImVec4(0.1f, 1.0f, 0.1f, 1.0f), "%s", "XBCentral - Multipurpose GUI application for Xbox 360 management");
        ImGui::Text("Copyright (c) 2026 'SpookScoop93' (@fuckingfuck81) \nLicensed under 3-Clause BSD License");
        ImGui::TextColored(ImVec4(0.1f, 1.0f, 0.1f, 1.0f), "%s", "Programming");
        ImGui::Text("SpookScoop93 - C++ programming");
        ImGui::Text("x4raynixx - C++ programming (help)");
        ImGui::TextColored(ImVec4(0.1f, 1.0f, 0.1f, 1.0f), "%s", "Tools used");
        ImGui::TextWrapped("extract-xiso - Xbox ISO extraction utility\n"
                           "Copyright (c) 2003 'in' (in@fishtank.com) / XboxDev\n"
                            "Licensed under the original BSD 4-Clause License.");
        ImGui::TextColored(ImVec4(0.1f, 1.0f, 0.1f, 1.0f), "%s", "Librares");
        ImGui::Text("ImGui, rlImGui and Raylib, imafilebrowser - GUI, Window Init and Raylib Integration for ImGui");
        ImGui::Text("C++ programming language and BASH shell/script language");

        ImGui::End();



        rlImGuiEnd();
        EndDrawing();
    }
    UnloadImage(logo);
    rlImGuiShutdown();
    CloseWindow();
    return 0;
}