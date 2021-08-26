//
//  Stack.cpp
//  lanUI
//
//  Created by René Descartes Domingos Muala on 29/07/21.
//  Copyright © 2021 René Descartes Domingos Muala. All rights reserved.
//

#include "Stack.hpp"
#include "../../Core/Core.hpp"

Stack::Stack(){
    first = last = nullptr;
    DrawableObject();
    fromColorScheme(Colors::Transparent, Colors::Transparent);
}

VStack& VStack::reload(){
    float width(0), height(0);
    if (!reloadingDisabled.get()) {
        for(Object* row = nextInZ.data; row ; row=row->nextInY.data)
        {
            width = (width < row->size.get().w + row->padding.get().left + row->padding.data.right) ? row->size.data.w + row->padding.data.left + row->padding.data.right : width;
            height+= row->size.data.h + row->padding.data.top + row->padding.data.bottom;
            row->size.leave();
            row->padding.leave();
        } set_size(width, height);//({0,0,width, height});
    } reloadingDisabled.leave();
    return (*this);
}

void VStack::fromList(std::list<Object*> objects){
    float width(0), height(0);
    last = nullptr;
    for(auto row : objects){
        //row->rootType.set(Object::VStackRoot);
        row->set_alignment(Object::Alignment::None);
        row->_useRootBounds();
        width = (width < row->size.get().w + row->padding.get().left + row->padding.data.right)
        ? row->size.data.w + row->padding.data.left + row->padding.data.right : width;
        height+= row->size.data.h + row->padding.data.top + row->padding.data.bottom;
        row->size.leave();
        row->padding.leave();
        if(!last) {
            embedInZ(*row);
            first = row;
        } else last->embedInY(*row);
        last = row;
    } set_size(width, height);
}

HStack& HStack::reload(){
    float width(0), height(0);
    if (!reloadingDisabled.get()) {
        for(Object* row = nextInZ.data; row ; row=row->nextInX.data)
        {
            width+= row->size.get().w + row->padding.get().left + row->padding.data.right;
            height= (height < row->size.data.h + row->padding.data.top + row->padding.data.top) ? row->size.data.h + row->padding.data.top + row->padding.data.top : height;
            row->size.leave();
            row->padding.leave();
        } set_size(width, height);
    } reloadingDisabled.leave();
    return (*this);
}

void HStack::fromList(std::list<Object*> objects){
    VStack();
    float width(0), height(0);
    last = nullptr;
    for(auto row : objects){
        //row->rootType.set(Object::HStackRoot);
        row->set_alignment(Object::Alignment::None);
        row->_useRootBounds();
        width+= row->size.get().w + row->padding.get().left + row->padding.data.right;
        height= (height < row->size.data.h + row->padding.data.top + row->padding.data.bottom) ? row->size.data.h + row->padding.data.top + row->padding.data.bottom : height;
        row->size.leave();
        row->padding.leave();
        if(!last) {
            embedInZ(*row);
            first = row;
        } else last->embedInX(*row);
        last = row;
    } set_size(width, height);
}

ZStack& ZStack::reload(){
    float width(0), height(0);
    if (!reloadingDisabled.get()) {
        for(Object* row = nextInZ.data; row ; row=row->nextInY.data)
        {
            width = (width < row->size.get().w + row->padding.get().left + row->padding.data.right) ? row->size.data.w + row->padding.data.left + row->padding.data.right : width;
            height = (height < row->size.data.h + row->padding.data.top + row->padding.data.bottom) ? row->size.data.h + row->padding.data.top + row->padding.data.bottom : height;
            row->size.leave();
            row->padding.leave();
        } set_size(width, height);
    } reloadingDisabled.leave();
    return (*this);
}

void ZStack::fromList(std::list<Object*> objects){
    float width(0), height(0);
    last = nullptr;
    for(auto row : objects){
        //row->rootType.set(Object::ZStackRoot);
        row->_useRootBounds();
        width = (width < row->size.get().w + row->padding.get().left + row->padding.data.right) ? row->size.data.w + row->padding.data.left + row->padding.data.right : width;
        height = (height < row->size.data.h + row->padding.data.top + row->padding.data.bottom) ? row->size.data.h + row->padding.data.top + row->padding.data.bottom : height;
        row->size.leave();
        row->padding.leave();
        if(!last) {
            embedInZ(*row);
            first = row;
        } else last->embedInZ(*row);
        last = row;
    } set_size(width, height);
}
