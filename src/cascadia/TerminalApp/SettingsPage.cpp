// Copyright (c) Microsoft Corporation
// Licensed under the MIT license.

#include "pch.h"
#include "SettingsPage.h"
#include "GlobalSettingsContent.h"
#include "KeyChordSerialization.h"
#include "ProfilesSettingsContent.h"
#include "ColorSchemesSettingsContent.h"
#include <winrt/Windows.UI.Xaml.Interop.h>

#include "SettingsPage.g.cpp"

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

    winrt::hstring SettingsPage::GetKeybindingString(int32_t var)
    {
        const auto temp = _settings->GetKeybindings().GetKeyBinding(static_cast<ShortcutAction>(var));
        return KeyChordSerialization::ToString(temp);
    }

    
    void SettingsPage::actionSelected(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::Controls::SelectionChangedEventArgs const& e)
    {
        
        auto selection = e.AddedItems().GetAt(0).as<Controls::ComboBoxItem>();
        auto selectionTag = selection.Tag();
        
        hstring actionHstring = unbox_value_or<hstring>(selectionTag, L"Default");
        std::string actionSelected = winrt::to_string(actionHstring);

        std::string action = actionSelected.substr(0, actionSelected.size() - 1);
        int actionIndex = stoi(actionSelected.substr(actionSelected.size() - 1));

        if (action == "newTabProfile")
        {
            newTabProfile().Text(GetKeybindingString(7 + actionIndex));
        }
        else if (action == "switchToTab")
        {
            switchToTab().Text(GetKeybindingString(24 + actionIndex));
        }
      
    }






}


