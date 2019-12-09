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
using namespace ::Input;
using namespace winrt::Microsoft::Terminal::Settings;

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

    // Method Description:
    // - Returns a key chord string based on the enum that matches the value passed
    // Arguments:
    // - actionIndex: int32_t that corrispopnds with a shortcut action
    // Return Value:
    // - hstring representing keychord
    winrt::hstring SettingsPage::GetKeybindingString(int32_t actionIndex)
    {
        const winrt::Microsoft::Terminal::Settings::KeyChord keyChord = _settings->GetKeybindings().GetKeyBinding(static_cast<ShortcutAction>(actionIndex));
        return KeyChordSerialization::ToString(keyChord);
    }

    // Method Description:
    // - Gets the hstring value of a vkey
    // Arguments:
    // - vkey: VirtualKey object
    // Return Value:
    // - a hstring that represents passed vkey
    winrt::hstring SettingsPage::VkeyToHstring(winrt::Windows::System::VirtualKey const vkey)
    {
        // define modifiers
        const int32_t ctrl = 17;
        const int32_t alt = 18;
        const int32_t shift = 16;

        // get enum vkey int
        const int32_t keyCode = static_cast<std::underlying_type<winrt::Windows::System::VirtualKey>::type>(vkey);
        hstring vkeyHstring = L"";
        //auto keyChord = winrt::Microsoft::Terminal::Settings::KeyChord(); // might need

        // check if a modifier
        if (ctrl == keyCode)
        {
            //keyChord.Modifiers(KeyModifiers::Ctrl); // might need
            vkeyHstring = L"ctrl";
        }
        else if (alt == keyCode)
        {
            vkeyHstring = L"alt";
        }
        else if (shift == keyCode)
        {
            vkeyHstring = L"shift";
        }
        // not modifier - serialize KeyChord
        else
        {
            winrt::Microsoft::Terminal::Settings::KeyChord keyChord(false, false, false, keyCode);
            vkeyHstring = KeyChordSerialization::ToString(keyChord);
        }

        return vkeyHstring;
    }

    // Method Description:
    // - Allows change of newTabProfile index or switchToTab index from ui dropdown
    // Arguments:
    // - sender: IInspectable ui element that triggered the event
    // - e: SelectionChangedEventArgs object with data from ui
    void SettingsPage::ActionSelected(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::Controls::SelectionChangedEventArgs const& e)
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

    // Method Description:
    // - fills textbox with key(s) pressed when key is released
    // Arguments:
    // - sender: IInspectable ui element that triggered the event
    // - args: KeyRoutedEventArgs object with data from ui
    void SettingsPage::PreviewKeyUpHandler(winrt::Windows::Foundation::IInspectable const& sender, Windows::UI::Xaml::Input::KeyRoutedEventArgs const& args)
    {
        // serialize key pressed to an hstring
        const winrt::Windows::System::VirtualKey vkey = args.Key();
        const hstring keyString = VkeyToHstring(vkey);

        const auto textbox_Inspectable = sender.as<Controls::TextBox>();

        // check if textbox empty
        if (textbox_Inspectable.Text() == L"")
        {
            _buffer = keyString;
            textbox_Inspectable.Text(keyString);
        }
        // key already entered in text
        else
        {
            _buffer = _buffer + L"+" + keyString;
            textbox_Inspectable.Text(_buffer);
        }

        // ensures nothing else tries to act on event
        args.Handled(true);
    }

    // disable textbox input
    void SettingsPage::PreviewKeyDownHandler(winrt::Windows::Foundation::IInspectable const& sender, Windows::UI::Xaml::Input::KeyRoutedEventArgs const& args)
    {
        args.Handled(true); 
    }

    // Method Description:
    // - loop through each command on the keybindings xaml ui saves the keybinding to the _settings object and exports it to json
    // Arguments:
    // - sender: IInspectable ui element that triggered the event (save button)
    // - args: RoutedEventArgs object with data from ui
    void SettingsPage::SaveKeybindings(winrt::Windows::Foundation::IInspectable const& sender, Windows::UI::Xaml::RoutedEventArgs const& args)
    {
        // loop through every keybinding on ui
        for (uint32_t x = 0; x < Keybindings().Children().Size(); ++x)
        {
            const auto keybindItem = Keybindings().Children().GetAt(x).as<Controls::TextBox>();
            const auto currTextBoxValue = keybindItem.Text(); // get string of whats in text box of current
            const auto keybindTag = keybindItem.Tag();

            // Makes sure were are replacing a text without any errors
            hstring keybindTagHstring = unbox_value_or<hstring>(keybindTag, L"Default"); // give value L"Default" if textbox empty
            if (keybindTagHstring != L"Default")
            {
                // check if this keybinding was changed
                if (currTextBoxValue != L"")
                {
                    //Get the index of the action to get the keybind pairing
                    int keybindIndex = stoi(winrt::to_string(keybindTagHstring));

                    // get shortcut action from enum
                    const auto currShortcutAction = static_cast<ShortcutAction>(keybindIndex);

                    // convert whats in text box to key chord
                    const auto key_chord = KeyChordSerialization::FromString(currTextBoxValue); 

                    // remove keybinding at command if present
                    // (ensure one to one mapping of comands to keybind for this ui design)
                    const Microsoft::Terminal::Settings::KeyChord currKeyBinding = _settings->GetKeybindings().GetKeyBinding(currShortcutAction);
                    _settings->GetKeybindings().ClearKeyBinding(currKeyBinding);

                    // set current key binding to _settings object
                    _settings->GetKeybindings().SetKeyBinding(currShortcutAction, key_chord);

                    // clear textbox (new placeholder should be element saved)
                    keybindItem.PlaceholderText(GetKeybindingString(x + 1));
                    keybindItem.Text(L"");
                }
            }
        }

        // get key bindings and send to string
        const winrt::TerminalApp::AppKeyBindings key_bindings = _settings->GetKeybindings();
        std::string json_string = winrt::to_string(key_bindings.ToString());
        const auto test_string = winrt::to_string(KeyChordSerialization::ToString(key_bindings.GetKeyBinding(ShortcutAction::CopyText)));

        // attempt to layer json (update profiles.json)
        // could not implement due to funky return types
        /*
        const AppKeyBindings key_bindings = _settings->GetKeybindings();
        auto keybindingsJson = key_bindings.ToJson();
        _settings->LayerJson(keybindingsJson);
        const auto test_string = winrt::to_string(KeyChordSerialization::ToString(key_bindings.GetKeyBinding(ShortcutAction::CopyText)));
        */

        // send key bindings string to json file in same location as profiles.json
        ofstream myfile;

        std::filesystem::path settingsPath{ CascadiaSettings::GetSettingsPath() };
        const auto folder = settingsPath.parent_path();

        string test = folder.string() + "\\keybindings.json";
        myfile.open(test);
        myfile << json_string;
        myfile.close();
    }

    // Method Description:
    // - Resets all ekybindings to what is in the defaults object
    // Arguments:
    // - sender: IInspectable ui element that triggered the event (reset to defaut button)
    // - args: RoutedEventArgs object with data from ui
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
            if (keybindTagHstring != L"Default")
            {
                //Get the index of the action to get the keybind pairing
                int keybindIndex = stoi(winrt::to_string(keybindTagHstring));

                // get shortcut action from enum
                const auto currShortcutAction = static_cast<ShortcutAction>(keybindIndex);

                // get the default binding for current shorcut action
                const auto keyChord = default_keybindings.GetKeyBinding(currShortcutAction);
                const winrt::hstring defaultBinding = KeyChordSerialization::ToString(keyChord);

                //Replace curent text with default keybinding
                keybindItem.Text(defaultBinding);
            }
        }
    }

}
