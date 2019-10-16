// Copyright (c) Microsoft Corporation.
// Licensed under the MIT license.

#include "pch.h"
#include "App.h"
#include "App.g.cpp"

using namespace winrt;
using namespace Windows::ApplicationModel;
using namespace Windows::ApplicationModel::Activation;
using namespace Windows::Foundation;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Navigation;

namespace winrt::TerminalApp::implementation
{
    App::App()
    {
        // This is the same trick that Initialize() is about to use to figure out whether we're coming
        // from a UWP context or from a Win32 context
        // See https://github.com/windows-toolkit/Microsoft.Toolkit.Win32/blob/52611c57d89554f357f281d0c79036426a7d9257/Microsoft.Toolkit.Win32.UI.XamlApplication/XamlApplication.cpp#L42
        const auto dispatcherQueue = ::winrt::Windows::System::DispatcherQueue::GetForCurrentThread();
        if (dispatcherQueue)
        {
            _isUwp = true;
        }

        Initialize();
    }

    static AppLogic& Logic()
    {
        static AppLogic logic;
        return logic;
    }

    /// <summary>
    /// Invoked when the application is launched normally by the end user.  Other entry points
    /// will be used such as when the application is launched to open a specific file.
    /// </summary>
    /// <param name="e">Details about the launch request and process.</param>
    void App::OnLaunched(LaunchActivatedEventArgs const& e)
    {
        // if this is a UWP... it means its our problem to hook up the content to the window here.
        if (_isUwp) 
        {
            auto content = Window::Current().Content();
            if (content == nullptr)
            {
                auto& logic = Logic();
                logic.LoadSettings();
                logic.Create();

                auto page = logic.GetRoot().as<TerminalPage>();

                Window::Current().Content(page);
                Window::Current().Activate();
            }
        }

        //Frame rootFrame{ nullptr };
        //auto content = Window::Current().Content();
        //if (content)
        //{
        //    rootFrame = content.try_as<Frame>();
        //}

        //// Do not repeat app initialization when the Window already has content,
        //// just ensure that the window is active
        //if (rootFrame == nullptr)
        //{
        //    // Create a Frame to act as the navigation context and associate it with
        //    // a SuspensionManager key
        //    rootFrame = Frame();

        //    //rootFrame.NavigationFailed({ this, &App::OnNavigationFailed });

        //    if (e.PreviousExecutionState() == ApplicationExecutionState::Terminated)
        //    {
        //        // Restore the saved session state only when appropriate, scheduling the
        //        // final launch steps after the restore is complete
        //    }

        //    if (e.PrelaunchActivated() == false)
        //    {
        //        if (rootFrame.Content() == nullptr)
        //        {
        //            AppLogic l;

        //            // When the navigation stack isn't restored navigate to the first page,
        //            // configuring the new page by passing required information as a navigation
        //            // parameter

        //            auto root = l.GetRoot();

        //            rootFrame.Navigate(xaml_typename<BlankApp1::MainPage>(), box_value(e.Arguments()));
        //        }

        //        // Place the frame in the current Window
        //        Window::Current().Content(rootFrame);
        //        // Ensure the current window is active
        //        Window::Current().Activate();
        //    }
        //}
        //else
        //{
        //    if (e.PrelaunchActivated() == false)
        //    {
        //        if (rootFrame.Content() == nullptr)
        //        {
        //            // When the navigation stack isn't restored navigate to the first page,
        //            // configuring the new page by passing required information as a navigation
        //            // parameter
        //            rootFrame.Navigate(xaml_typename<BlankApp1::MainPage>(), box_value(e.Arguments()));
        //        }
        //        // Ensure the current window is active
        //        Window::Current().Activate();
        //    }
        //}
    }
}
