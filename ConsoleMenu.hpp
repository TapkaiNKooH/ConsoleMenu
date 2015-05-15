#ifndef CONSOLEMENU_HPP
#define CONSOLEMENU_HPP

#include <iostream>
#include <fstream>
#include <functional>
#include <vector>
#include <cstdlib>
#include <map>
#include <algorithm>
#include <conio.h>

namespace CLI {
using namespace std;

class ConsoleMenu;

struct MenuItem {
    typedef function <void (ConsoleMenu&)> Callback;
    MenuItem (const string& cap, Callback cal) : caption (cap), callback (cal) {}

    string caption;
    Callback callback;
};

class ConsoleMenu {
public:
    typedef map <string, char> KeyMap;

    ConsoleMenu () : mCurrentItemIndex (0), mRunning (false), mCursor ("> ") {
        mKeyMap["up"]     = 'w';
        mKeyMap["down"]   = 's';
        mKeyMap["action"] = 'q';
    }

    void addMenuItem (MenuItem item) {
        mMenuItems.push_back (item);
    }

    void addMenuItem (const string& cap, MenuItem::Callback call) {
        mMenuItems.push_back (MenuItem (cap, call));
    }

    void setCursor (const string& c) {
        mCursor = c;
    }


    MenuItem& getMenuItem (int index) {
        return mMenuItems.at (index);
    }

    MenuItem& operator[] (int index) {
        return getMenuItem (index);
    }

    MenuItem& operator[] (const string& caption) {
        return getMenuItem (caption);
    }

    MenuItem& getMenuItem (const string& caption) {
        return *mFindItem (caption);
    }

    void deleteMenuItem (const string& caption) {
        mMenuItems.erase (mFindItem (caption));
    }

    void deleteMenuItem (int index) {
        getMenuItem (index);
        mMenuItems.erase (mMenuItems.begin() + index);
    }

    void clear() {
        for (int i = 0; i < mMenuItems.size(); i++)
            deleteMenuItem (i);
    }

    void run() {
        mRunning = true;
        while (mRunning) {
            mDraw();
            mInput();
        }
    }

    void quit() {
        mRunning = false;
    }

    void keyMap (const string& l, char r) {
        mKeyMap.at (l) = r;
    }

    void setTitle (const string& title) {
        mTitle = title;
    }

    MenuItem& getCurrentItem() {
        return getMenuItem (mCurrentItemIndex);
    }

private:
    vector <MenuItem>::iterator mFindItem (const string& caption) {
        auto found = find_if (mMenuItems.begin(), mMenuItems.end(), [&] (const MenuItem& f) { return f.caption == caption; });
        if (found != mMenuItems.end())
            return found;
        else throw runtime_error ("Couldn't find menu item '" + caption + "', reason: item doesn't exist!");
        return mMenuItems.begin();
    }

    void mInput() {
        char key = getch();

        if (key == mKeyMap["up"]) {
            if (mCurrentItemIndex == 0)
                mCurrentItemIndex = mMenuItems.size() - 1;
            else
                mCurrentItemIndex--;
        }

        if (key == mKeyMap["down"]) {
            if (mCurrentItemIndex == mMenuItems.size() - 1)
                mCurrentItemIndex = 0;
            else
                mCurrentItemIndex++;
        }

        //mCurrentItem = mMenuItems [mCurrentItemIndex];
        if (key == mKeyMap["action"])
            getCurrentItem().callback (*this);
    }

    void mDraw() {
        system ("cls");
        if (mTitle.size()) {
            for (int i = 0; i < 3; i++) {
                cout << "*";
                for (auto& ch : mTitle) {
                    if (i == 1) cout << ch;
                    else        cout << "*";
                }
                cout << "*" << endl;
            }
            cout << endl;
        }

        int i = 0;
        for (auto& item : mMenuItems) {
            if (i == mCurrentItemIndex)
                cout << mCursor;
            cout << item.caption << endl;
            i++;
        }
    }

private:
    vector <MenuItem> mMenuItems;
    string mTitle;
    string mCursor;
    KeyMap mKeyMap;
    int mCurrentItemIndex;
    bool mRunning;
};

}

#endif // CONSOLEMENU_HPP
