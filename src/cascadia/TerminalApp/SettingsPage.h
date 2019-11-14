// Copyright (c) Microsoft Corporation.
// Licensed under the MIT license.

#pragma once

#include "winrt/Microsoft.UI.Xaml.Controls.h"

#include "SettingsPage.g.h"
#include "CascadiaSettings.h"

namespace winrt::TerminalApp::implementation
{
    struct SettingsPage : SettingsPageT<SettingsPage>
    {
    public:
        SettingsPage();
        SettingsPage(std::shared_ptr<::TerminalApp::CascadiaSettings> settings);
        winrt::hstring GetKeybindingString(int32_t var);
    private:
        std::shared_ptr<::TerminalApp::CascadiaSettings> _settings{ nullptr };
    };
}

namespace winrt::TerminalApp::factory_implementation
{
    struct SettingsPage : SettingsPageT<SettingsPage, implementation::SettingsPage>
    {
    };
}
