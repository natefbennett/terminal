// Copyright (c) Microsoft Corporation
// Licensed under the MIT license.

#include "pch.h"
#include "SettingsPage.h"
#include "GlobalSettingsContent.h"
#include "KeyChordSerialization.h"
#include "ProfilesSettingsContent.h"
#include "ColorSchemesSettingsContent.h"
#include "CascadiaSettings.h"
#include <winrt/Windows.UI.Xaml.Interop.h>
#include <fstream>
#include <iostream>
#include "SettingsPage.g.cpp"

using namespace std;
using namespace winrt;
using namespace Windows::UI::Xaml;
using namespace ::TerminalApp;

namespace winrt::TerminalApp::implementation
{
    SettingsPage::SettingsPage()
    {
        InitializeComponent();
    }

    SettingsPage::SettingsPage(std::shared_ptr<::TerminalApp::CascadiaSettings> settings)
    {
        InitializeComponent();

        _settings = settings;
    }

    // Returns a string based on the enum that matches the int value passed
    winrt::hstring SettingsPage::GetKeybindingString(int32_t var)
    {
        const auto temp = _settings->GetKeybindings().GetKeyBinding(static_cast<ShortcutAction>(var));
        return KeyChordSerialization::ToString(temp);
    }

    
    void SettingsPage::actionSelected(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::Controls::SelectionChangedEventArgs const& e)
    {
        
        auto selectionTag = e.AddedItems().GetAt(0).as<Controls::ComboBoxItem>().Tag();
        hstring actionHstring = unbox_value_or<hstring>(selectionTag, L"Default");
        int actionIndex = stoi(winrt::to_string(actionHstring));

        auto senderTag = sender.as<Controls::ComboBox>().Tag();
        hstring action = unbox_value_or<hstring>(senderTag, L"Default");
       

        if (action == L"newTabProfile")
        {
            const int index = 7 + actionIndex;
            const hstring indexHString = winrt::to_hstring(index);

            //Set text and tag
            newTabProfile_keybind().Text(GetKeybindingString(index));
            newTabProfile_keybind().Tag(winrt::box_value(indexHString));
        }
        else if (action == L"switchToTab")
        {
            const int index = 24 + actionIndex;
            const hstring indexHString = winrt::to_hstring(index);

            //Set text and tag
            switchToTab_keybind().Text(GetKeybindingString(index));
            switchToTab_keybind().Tag(winrt::box_value(indexHString));

        }

      
    }

    // Goes through each command on the keybindings xaml ui and exports it to json
    void SettingsPage::SaveKeybindings(winrt::Windows::Foundation::IInspectable const& sender, Windows::UI::Xaml::RoutedEventArgs const& args)
    {
        const auto key_bindings = _settings->GetKeybindings();
        std::string json_string = winrt::to_string(key_bindings.ToString());

        ofstream myfile;
        myfile.open("C:/Users/josep/Documents/example.txt");
        myfile << json_string;
        myfile.close();
    }

    void SettingsPage::ResetToDefault(winrt::Windows::Foundation::IInspectable const& sender, Windows::UI::Xaml::RoutedEventArgs const& args)
    {
         //Getting the default keybindings
        const auto defaults = CascadiaSettings::LoadDefaults();
        const winrt::TerminalApp::AppKeyBindings default_keybindings = defaults->GetKeybindings();

        //Go through every keybinding 
        for (uint32_t x = 0; x < Keybindings().Children().Size(); x++)
        {
            const auto keybindItem = Keybindings().Children().GetAt(x).as<Controls::TextBox>();
            const auto keybindTag = keybindItem.Tag();
            hstring keybindTagHstring = unbox_value_or<hstring>(keybindTag, L"Default");

            //Makes sure were are replacing a text without any errors
            //The textbox might be empty so we will not replace text when no selection is made on a dropdown
            if (keybindTagHstring != L"Default")
            {
                //Get the index of the action to get the keybind pairing
                int keybindIndex = stoi(winrt::to_string(keybindTagHstring));

                //Replace curent text with default keybinding
                keybindItem.Text(GetKeybindingString(keybindIndex));
            }

        
        }
    }

}


