//
//  Stack.cpp
//  lanUI
//
//  Created by René Descartes Domingos Muala on 29/07/21.
//  Copyright © 2021 René Descartes Domingos Muala. All rights reserved.
//

#include "Stack.hpp"

BSStack::BSStack(){
    first = last = nullptr;
    fromColorScheme(Colors::Transparent, Colors::Transparent);
}

void BSStack::free(){
    nextInZ.hold();
    Object* row = nextInZ.data;
    if(row) row->_delete_tree();
    first = last = nullptr;
    nextInZ.leave();
}

BSStack& BSVStack::reload(){
    float width(0), height(0);
    if (!reloadingDisabled.get()) {
        nextInZ.hold();
        for(Object* row = nextInZ.data; row ; row=row->nextInY.data)
        {
            width = (width < row->size.get().w + row->padding.get().left + row->padding.data.right) ?
            row->size.data.w + row->padding.data.left + row->padding.data.right : width;
            height+= row->size.data.h + row->padding.data.top + row->padding.data.bottom;
            row->size.leave();
            row->padding.leave();
        }
        nextInZ.leave();
        set_size(width, height);//({0,0,width, height});
    }
    reloadingDisabled.leave();
    return (*this);
}

Object * BSVStack::requestObject(size_t index){
    Object * ptr = nextInZ.get();
    Object * last_ptr = nullptr;
    size_t subIndex = 0;
    while (ptr && subIndex < index) {
        last_ptr = ptr;
        ptr = ptr->nextInY.get();
        last_ptr->nextInY.leave();
        subIndex++;
    } nextInZ.leave();
    if(ptr && subIndex == index)
        return ptr;
    else
        return nullptr;
}

BSStack BSVStack::fromList(std::list<Object*> objects){
    float width(0), height(0);
    unsigned long index(0);
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
        row->index.set(index);
        if(!last) {
            embedInZ(*row);
            first = row;
        } else last->embedInY(*row);
        last = row;
    }
    if(!reloadingDisabled.get()){
        reloadingDisabled.leave();
        set_size(width, height);
    } else
        reloadingDisabled.leave();

    return (*this);
}

BSStack& BSHStack::reload(){
    float width(0), height(0);
    if (!reloadingDisabled.get()) {
        nextInZ.hold();
        for(Object* row = nextInZ.data; row ; row=row->nextInX.data)
        {
            width+= row->size.get().w + row->padding.get().left + row->padding.data.right;
            height= (height < row->size.data.h + row->padding.data.top + row->padding.data.top) ? row->size.data.h + row->padding.data.top + row->padding.data.top : height;
            row->size.leave();
            row->padding.leave();
        }
        nextInZ.leave();
        set_size(width, height);
    } reloadingDisabled.leave();
    return (*this);
}

Object * BSHStack::requestObject(size_t index){
    Object * ptr = nextInZ.get();
    Object * last_ptr = nullptr;
    size_t subIndex = 0;
    while (ptr && subIndex < index) {
        last_ptr = ptr;
        ptr = ptr->nextInX.get();
        last_ptr->nextInX.leave();
        subIndex++;
    } nextInZ.leave();
    
    if(ptr && subIndex == index)
        return ptr;
    else
        return nullptr;
}

BSStack BSHStack::fromList(std::list<Object*> objects){
    float width(0), height(0);
    unsigned long index(0);
    last = nullptr;
    for(auto row : objects){
        //row->rootType.set(Object::HStackRoot);
        row->set_alignment(Object::Alignment::None);
        row->_useRootBounds();
        width+= row->size.get().w + row->padding.get().left + row->padding.data.right;
        height= (height < row->size.data.h + row->padding.data.top + row->padding.data.bottom) ? row->size.data.h + row->padding.data.top + row->padding.data.bottom : height;
        row->size.leave();
        row->padding.leave();
        row->index.set(index);
        if(!last) {
            embedInZ(*row);
            first = row;
        } else last->embedInX(*row);
        last = row;
    } if(!reloadingDisabled.get()){
        reloadingDisabled.leave();
        set_size(width, height);
    } else
        reloadingDisabled.leave();
    return (*this);
}

BSStack& BSZStack::reload(){
    float width(0), height(0);
    if (!reloadingDisabled.get()) {
        nextInZ.hold();
        for(Object* row = nextInZ.data; row ; row=row->nextInY.data)
        {
            width = (width < row->size.get().w + row->padding.get().left + row->padding.data.right) ? row->size.data.w + row->padding.data.left + row->padding.data.right : width;
            height = (height < row->size.data.h + row->padding.data.top + row->padding.data.bottom) ? row->size.data.h + row->padding.data.top + row->padding.data.bottom : height;
            row->size.leave();
            row->padding.leave();
        }
        nextInZ.leave();
        set_size(width, height);
    } reloadingDisabled.leave();
    return (*this);
}

Object * BSZStack::requestObject(size_t index){
    Object * ptr = nextInZ.get();
    Object * last_ptr = nullptr;
    size_t subIndex = 0;
    while (ptr && subIndex < index) {
        last_ptr = ptr;
        ptr = ptr->nextInZ.get();
        last_ptr->nextInZ.leave();
        subIndex++;
    } nextInZ.leave();
    if(ptr && subIndex == index)
        return ptr;
    else
        return nullptr;
}

BSStack BSZStack::fromList(std::list<Object*> objects){
    float width(0), height(0);
    unsigned long index(0);
    last = nullptr;
    for(auto row : objects){
        //row->rootType.set(Object::ZStackRoot);
        row->_useRootBounds();
        width = (width < row->size.get().w + row->padding.get().left + row->padding.data.right) ? row->size.data.w + row->padding.data.left + row->padding.data.right : width;
        height = (height < row->size.data.h + row->padding.data.top + row->padding.data.bottom) ? row->size.data.h + row->padding.data.top + row->padding.data.bottom : height;
        row->size.leave();
        row->padding.leave();
        row->index.set(index);
        if(!last) {
            embedInZ(*row);
            first = row;
        } else last->embedInZ(*row);
        last = row;
    } if(!reloadingDisabled.get()){
        reloadingDisabled.leave();
        set_size(width, height);
    } else
        reloadingDisabled.leave();
    return (*this);
}
