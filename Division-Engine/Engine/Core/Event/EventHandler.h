#ifndef EVENTHANDLER_H
#define EVENTHANDLER_H

// This is EventHandler Interface class
class EventHandler {
public:
	virtual void WindowResizeCallback(const int width_, const int height_) {};
	virtual void KeyCallback(const int key_, const int scanCode_, const int action_, const int mods_) {};
	virtual void CursorPositionCallback(const double xpos_, const double ypos_) {};
	virtual void MouseButtonCallback(const int button_, const int action_, const int mods_) {};
};

#endif // !EVENTHANDLER_H
