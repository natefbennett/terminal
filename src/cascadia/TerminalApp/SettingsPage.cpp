// Copyright (c) Microsoft Corporation
// Licensed under the MIT license.

#include "pch.h"
#include "SettingsPage.h"
#include "GlobalSettingsContent.h"
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
        winrt::hstring keybinding{ L"Hello, World!" };
        return keybinding;
    }
}
