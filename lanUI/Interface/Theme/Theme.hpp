//
//  Theme.hpp
//  lanUI
//
//  Created by René Descartes Domingos Muala on 26/11/21.
//  Copyright © 2021 René Descartes Domingos Muala. All rights reserved.
//

#ifndef Theme_hpp
#define Theme_hpp

#include <stdio.h>
#include "../Color/Color.hpp"
#include "../Pallete/Pallete.hpp"

#include "../Image/Image.hpp"
#include "../Stack/Stack.hpp"
#include "../List/List.hpp"
#include "../View/View.hpp"
#include "../Paragraph/Paragraph.hpp"
#include "../TextField/TextField.hpp"
#include "../Container/Container.hpp"
#include "../Spacer/Spacer.hpp"
#include "../Button/Button.hpp"

class Theme {
public:
    
    /* TextStyles for this theme must to be defined at ThemeTextStyles.hpp
     */

    class ThemeColors {
        // by default lanui will only use the first three colors for surface and accent in interal routines
        Pallete<10> surface, text, accent;
    public:
        
        template<PalleteElement element>
        const Color get_surface_color() const {
            return surface[element];
        }
        
        template<PalleteElement element>
        const Color get_text_color() const {
            return text[element];
        }
        
        template<PalleteElement element>
        const Color get_accent_color() const {
            return accent[element];
        }
        friend Theme;
    }
    /// Theme colors
    colors;
    
    class Image  : public BSImage  {};
    class Stack  : public BSStack  {};
    class VStack : public BSVStack {};
    class HStack : public BSHStack {};
    class ZStack : public BSZStack {};
    class List   : public BSList   {};
    class View   : public BSView   {};
    class Paragraph : public BSParagraph {};
    class TextField : public BSTextField {
    public:
        TextField(const std::string placeholder = ""): BSTextField(placeholder){}
        TextField(BSTextField const & other): BSTextField(other){}
    };
    class Container : public BSContainer {
    public:
        Container(Object& any){
            set_content(any);
        }
    };
    class Spacer    : public BSSpacer {};
    class Button    : public BSButton {
        Text * child;
        bool heap_child;
    public:
        virtual void default_init();
        Button(Object &);
        Button(Text &);
        Button(std::string const);
        ~Button();
    };
        
    virtual void _set_theme_colors();
    virtual void _set_default_font() const;
    
    Theme(){
        _set_theme_colors();
    }
};

namespace Themes {
    class _default_theme_type : public Theme {};
    const _default_theme_type _default;
}

typedef Themes::_default_theme_type::Image   Image;
typedef Themes::_default_theme_type::Stack   Stack;
typedef Themes::_default_theme_type::VStack  VStack;
typedef Themes::_default_theme_type::HStack  HStack;
typedef Themes::_default_theme_type::ZStack  ZStack;
typedef Themes::_default_theme_type::List    List;
typedef Themes::_default_theme_type::View    View;
typedef Themes::_default_theme_type::Paragraph   Paragraph;
typedef Themes::_default_theme_type::TextField   TextField;
typedef Themes::_default_theme_type::Container   Container;
typedef Themes::_default_theme_type::Spacer   Spacer;
typedef Themes::_default_theme_type::Button   Button;

#endif /* Theme_hpp */
