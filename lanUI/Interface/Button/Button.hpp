//
//  Button.hpp
//  lanUI
//
//  Created by René Descartes Domingos Muala on 08/08/21.
//  Copyright © 2021 René Descartes Domingos Muala. All rights reserved.
//

#ifndef Button_hpp
#define Button_hpp

#include "../Object/Object.hpp"

/** A Button
 */
class BSButton : public InteractiveObject {
public:
    BSButton(Object* content = nullptr){
        InteractiveObject();
        if(content) set_content(*content);
    }
};

#endif /* Button_hpp */
