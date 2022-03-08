//
//  PathResolver.hpp
//  lanUI
//
//  Created by René Descartes Domingos Muala on 29/01/22.
//  Copyright © 2022 René Descartes Domingos Muala. All rights reserved.
//

#ifndef PathResolver_hpp
#define PathResolver_hpp

#include <stdio.h>
#include <string>
#include <regex>
#include <list>
#include <utility>
#include "../Settings.hpp"

namespace PathResolverData {

const std::list<const std::pair<const std::regex, const std::string>> macros = {
    /*
     Hint: Create your path-macro by adding a line following the scruch bellow:
     
        {std::regex("example:", std::regex_constants::icase), (std::string(LUI::BUNDLE_NAME)+"/Example/path")},
     
    */
    
    // Add your macro here
    {
        std::regex("user:", std::regex_constants::icase), (std::string(LUI::BUNDLE_NAME)+"/User/")
    },
    {
        std::regex("(cache|tmp):", std::regex_constants::icase), (std::string(LUI::BUNDLE_NAME)+ "/User/Cache/")
    },
    {std::regex("(license|licenses):", std::regex_constants::icase), (std::string(LUI::BUNDLE_NAME)+"/User/License/")
    },
    {std::regex("(settings|configs):", std::regex_constants::icase), (std::string(LUI::BUNDLE_NAME)+"/User/Settings/")
    },
    {std::regex("(system|software):", std::regex_constants::icase), (std::string(LUI::BUNDLE_NAME)+"/System/")
    },
    {std::regex("library:", std::regex_constants::icase), (std::string(LUI::BUNDLE_NAME)+"/System/Library/")
    },
    {std::regex("fonts:", std::regex_constants::icase), (std::string(LUI::BUNDLE_NAME)+"/System/Library/Fonts/")
    },
    {std::regex("(dependences|libraries):", std::regex_constants::icase), (std::string(LUI::BUNDLE_NAME)+"/System/Dependences/")
    },
    {std::regex("(resources|media):", std::regex_constants::icase), (std::string(LUI::BUNDLE_NAME)+"/System/Resources/")
    },
};
}

struct PathResolver {
    ///  converts UNIX Paths  to equivalent DOS Paths
    static std::string UNIX(std::string str){
        for(auto & symb : str){
            if(symb == '\\')
                symb = '/';
        } return str;
    }
    
    ///  converts UNIX Paths  to equivalent DOS Paths
    static std::string DOS(std::string str){
        for(auto & symb : str){
            if(symb == '/')
                symb = '\\';
        } return str;
    }
    
    /// resolves path macros in UNIX/DOS format
    static std::string resolve(std::string str){
        for (auto macro : PathResolverData::macros){
            if(std::regex_search(str, macro.first)){
                str = std::regex_replace(str.c_str(), macro.first, macro.second);
            }
        }
#if (defined(__WIN32__) || defined(__WINRT__))
        return DOS(str);
#else
        return UNIX(str);
#endif
    }
};

#endif /* PathResolver_hpp */
