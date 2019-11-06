// Copyright (c) Microsoft Corporation.
// Licensed under the MIT license.

#pragma once
#include "AzureConnection.g.h"
#include "pch.h"

namespace winrt::Microsoft::Terminal::TerminalConnection::implementation
{
    struct AzureConnection : AzureConnectionT<AzureConnection>
    {
        AzureConnection() = default;

        AzureConnection(uint32_t rows, uint32_t columns);
        static bool IsAzureConnectionAvailable();
        winrt::event_token TerminalOutput(Microsoft::Terminal::TerminalConnection::TerminalOutputEventArgs const& handler);
        void TerminalOutput(winrt::event_token const& token);
        winrt::event_token StateChanged(StateChangedEventArgs const& handler);
        void StateChanged(winrt::event_token const& token);
        void Start();
        void WriteInput(hstring const& data);
        void Resize(uint32_t rows, uint32_t columns);
        void Close();
    };
}
namespace winrt::Microsoft::Terminal::TerminalConnection::factory_implementation
{
    struct AzureConnection : AzureConnectionT<AzureConnection, implementation::AzureConnection>
    {
    };
}
