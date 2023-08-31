PARENT WINDOW
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Extended window style
    Specifies additional extended window styles for the window.
    Extended window styles are specific attributes that enhance the appearance and behavior of the window, such as layered rendering or customizations.

Window Class Name
    Provides the name of the window class that the new window belongs to.
    This name should match the class name registered using RegisterClass.

Window Title
    Sets the text displayed in the title bar of the window.

Window Style
    Defines the basic style of the window, including features like the title bar, system menu, and buttons.
    The WS_OVERLAPPEDWINDOW style includes multiple predefined styles.

Initial Position (Wdith and Height)
    Specifies the initial position of the window on the screen.
    Constants like CW_USEDEFAULT allow the OS to determine the position.

Height

Width

Parent Window
    Specifies the parent window of the new window.
    A NULL value indicates the window has no parent.

Menu
    Specifies the menu associated with the window.
    A NULL value indicates no menu is associated.

Instance Handle
    Provides the handle to the application instance that's creating the window.
    Helps identify which application is responsible for the window.
    
Additional Data
    Can be used to pass extra data to the window procedure.
    In this case, NULL indicates no additional data is passed.




Input WINDOW
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Extended Window Style (WS_EX_CLIENTEDGE):
Specifies additional extended window styles for the window. In this case, WS_EX_CLIENTEDGE is used, which adds a sunken edge to the window to create a visual separation between the client area and the non-client area.

Window Class Name ("EDIT"):
Specifies the name of the window class to be created. In this case, "EDIT" refers to the predefined class for an edit control, which is used to create a text input box.

Initial Text (""):
Provides the initial text that will appear inside the window. In this case, it's an empty string, so the input box starts with no text.

Window Style (WS_CHILD | WS_VISIBLE | WS_BORDER | WS_VSCROLL | ES_MULTILINE):
Defines the basic style of the window. The combination of flags sets various styles for the input text box:

WS_CHILD: Indicates that the window is a child window.
WS_VISIBLE: Makes the window initially visible.
WS_BORDER: Adds a border around the window.
WS_VSCROLL: Adds a vertical scrollbar to the window if necessary.
ES_MULTILINE: Enables multiline editing, allowing text to be entered on multiple lines.
Initial Position and Size (10, 10, 0.6 * 600, 100):
Specifies the initial position (x, y) and size (width, height) of the window. In this case, the input text box is positioned at (10, 10) with a width of 60% of the main window's width and a fixed height of 100 pixels.

Parent Window (hwnd):
Specifies the parent window of the new window. In this case, the main window created earlier (hwnd) serves as the parent.

Menu Handle (NULL):
Specifies the menu associated with the window. In this case, no menu is associated, so it's set to NULL.

Instance Handle (hInstance):
Provides the handle to the application instance that's creating the window. It helps identify which application is responsible for the window.

Additional Data (NULL):
Can be used to pass extra data to the window procedure. In this case, no additional data is passed, so it's set to NULL.



WindowProc Params
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

HWND hwnd:

represents the handle of the window that the message is directed to.

UINT uMsg:
Message code, an unsigned integer that identifies the type of message being processed.

WPARAM wParam:
word parameter, used to carry additional information about the message.

LPARAM lParam:
long parameter, like wParam, is used to carry additional information about the message.