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
        
        auto selectionTag = e.AddedItems().GetAt(0).as<Controls::ComboBoxItem>().Tag();
        hstring actionHstring = unbox_value_or<hstring>(selectionTag, L"Default");
        int actionIndex = stoi(winrt::to_string(actionHstring));

        auto senderTag = sender.as<Controls::ComboBox>().Tag();
        hstring action = unbox_value_or<hstring>(senderTag, L"Default");
       

        if (action == L"newTabProfile")
        {
            newTabProfile_keybind().Text(GetKeybindingString(7 + actionIndex));
        }
        else if (action == L"switchToTab")
        {
            switchToTab_keybind().Text(GetKeybindingString(24 + actionIndex));
        }

      
    }

    void SettingsPage::SaveKeybindings(winrt::Windows::Foundation::IInspectable const& sender, Windows::UI::Xaml::RoutedEventArgs const& args)
    {
        
    }

    void SettingsPage::textbox_KeyDown(winrt::Windows::Foundation::IInspectable const& sender, Windows::UI::Xaml::Input::TappedRoutedEventArgs const& e)
    {
        //implementation
        throw hresult_not_implemented();
    }

    void SettingsPage::textbox_Loaded(winrt::Windows::Foundation::IInspectable const& sender, Windows::UI::Xaml::RoutedEventArgs const& e)
    {
        this->AddHandler(UIElement::KeyDownEvent, KeyDown( &SettingsPage::textbox_KeyDown ), true);
    }

}


