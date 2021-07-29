//
//  Stack.cpp
//  lanUI
//
//  Created by René Descartes Domingos Muala on 29/07/21.
//  Copyright © 2021 René Descartes Domingos Muala. All rights reserved.
//

#include "Stack.hpp"
#include "../../Core/Core.hpp"

const int Stack::lenght(){
    const int len = count.get();
    count.leave();
    return len;
}

Stack::Stack(){
    DrawableObject();
    fromColorScheme(Colors::Transparent, Colors::Transparent);
}

void VStack::fromList(std::list<Object*> objects){
    Object * last(nullptr);
    float width(0), height(0);
    for(auto row : objects){
        row->set_alignment(Object::Alignment::None);
        row->useRootBounds();
        width= (width < row->size.get().w + row->padding.get().left + row->padding.data.right) ? row->size.data.w + row->padding.data.left + row->padding.data.right : width;
        height+= row->size.data.h + row->padding.data.top + row->padding.data.top;
        row->size.leave();
        row->padding.leave();
        if(!last) embedInZ(*row);
        else last->embedInY(*row);
        last = row;
    } size.set({0,0,width, height});
}

void HStack::fromList(std::list<Object*> objects){
    VStack();
    Object * last(nullptr);
    float width(0), height(0);
    for(auto row : objects){
        row->set_alignment(Object::Alignment::None);
        row->useRootBounds();
        width+= row->size.get().w + row->padding.get().left + row->padding.data.right;
        height= (height < row->size.data.h + row->padding.data.top + row->padding.data.top) ? row->size.data.h + row->padding.data.top + row->padding.data.top : height;
        row->size.leave();
        row->padding.leave();
        if(!last) embedInZ(*row);
        else last->embedInX(*row);
        last = row;
    } size.set({0,0,width, height});
}

void ZStack::fromList(std::list<Object*> objects){
    Object * last(nullptr);
    float width(0), height(0);
    for(auto row : objects){
        row->set_alignment(Object::Alignment::None);
        row->useRootBounds();
        width= (width < row->size.get().w + row->padding.get().left + row->padding.data.right) ? row->size.data.w + row->padding.data.left + row->padding.data.right : width;
        height= (height < row->size.data.h + row->padding.data.top + row->padding.data.top) ? row->size.data.h + row->padding.data.top + row->padding.data.top : height;
        row->size.leave();
        row->padding.leave();
        if(!last) embedInZ(*row);
        else last->embedInZ(*row);
        last = row;
    } size.set({0,0,width, height});
}
