//
//  aves.cpp
//  lanUI
//
//  Created by René Descartes Domingos Muala on 24/09/21.
//  Copyright © 2021 René Descartes Domingos Muala. All rights reserved.
//

#include <iostream>
#ifdef USING_LANUI_CMAKE_BUILDER
// Use the generated template headers
#include "../Template/lanUI/lanUI.hpp"
#else
#include "../lanUI/lanUI.hpp"
#endif

/*
 Note:
    On Xcode, it will be necessary to disable metal api validation.
    Otherwise your application will immediately crash after resizing the window.
    (Disable it on: Scheme > Edit Scheme... > Run > Diagnostics or Options > Metal API Validation).
 */

class BirdView : public View {
public:
    
    VStack contentStack;
    Container contentStackContainer = Container(contentStack);
    HStack mainStack;
    Text title;
    Image bird;
    Container birdContainer = Container(bird);
    Paragraph description;
    VStack descriptionContainer;
    
    Text forwardTxt;
    Text backwardTxt;
    Button forwardBtn = Button(forwardTxt);
    Button backwardBtn = Button(backwardTxt);
    
    std::string birdName;
    std::string birdDescription;
    std::string imagePath;
    View * last;
    View * next;

    BirdView(Window & win, std::string name, std::string _description, std::string path, View * last, View * next): birdName(name), birdDescription(_description), imagePath(path), last(last), next(next) {
        create_silently(win);
        set_size(1100, 600);
        set_alignment(Center);
        fromColorScheme();
    }
    
    void createTitle(Window& window){
        title.from_string(birdName);
        title.set_alignment(Left);
        title.bold(18);
        title.set_foreground_color(Colors::Orange);
        title.set_padding({5,0});
    }
    
    void createBird(Window& window){
        bird.fromFile(imagePath.c_str(), window.sdlRenderer.get());
        window.sdlRenderer.leave();
        bird.set_size(512, 512);
        bird.set_alignment(Center);
        bird.set_default_animation(1, CallbackExpr({
            // since we are using static values here, they will be shared with all views
            static float size = 512;
            static bool increase = true;
            
            if(increase && size < 530)          size+=0.1;
            else if(!increase && size > 500)    size-=0.1;
            else                                increase = !increase;
            bird.set_size(size, size);
            return true;
        }));
        birdContainer.set_size(512, 512);
        birdContainer.disable_reloading();
    }
    
    void createDescription(Window& window){
        std::stringstream stream;
        
        createTitle(window);
        
        stream << "%size:14 " << birdDescription;
        
        description.from_stringstream(stream, Paragraph::Wrapper::Char, 60);
        description.set_alignment(Left);
        descriptionContainer.fromList(std::list<Object *>{&title, &description});
        descriptionContainer.set_size(525, 160);
        descriptionContainer.disable_reloading();
    }
    
    void createButtons(Window& window){
        forwardTxt.from_string("Próxima");
        if(next)
            forwardTxt.set_foreground_color(Colors::Orange);
        else
            forwardTxt.set_foreground_color(Colors::Orange.from_a(50));
        forwardTxt.bold(14);
        forwardTxt.set_alignment(Center);
        forwardBtn.Object::set_size(512, 20);
        forwardBtn.disable_reloading();
        forwardBtn.set_padding({5,5});
        
        backwardTxt.from_string("Anterior");
        backwardTxt.set_foreground_color(Colors::Orange);
        backwardTxt.bold(14);
        backwardTxt.set_alignment(Center);
        backwardBtn.Object::set_size(512, 20);
        backwardBtn.disable_reloading();
        backwardBtn.set_padding({5,5});
    }
    
    void createContentStack(Window& window){
        contentStack.fromList(std::list<Object *>{&descriptionContainer, &backwardBtn, &forwardBtn});
        contentStack.set_alignment(Center);
        contentStackContainer.set_size(600, 512);
        contentStackContainer.disable_reloading();
    }
    
    void createAnimation(){
        set_default_animation(5, CallbackExpr({
            if(next){
                forwardTxt.set_foreground_color(Colors::Orange);
                forwardBtn.on_click(CallbackExpr(
                                                 window->set_view(*next);
                                                 )
                                    );
            } else {
                  forwardTxt.set_foreground_color(Colors::Orange.from_a(50));
            }
            if(last){
                backwardBtn.on_click(CallbackExpr(
                                                 window->set_view(*last);
                                                 )
                                    );
            }
            return true;
        }));
    }
    
    Object& body(Window& window) override {
        createBird(window);
        createDescription(window);
        createButtons(window);
        createContentStack(window);
        createAnimation();
        mainStack.fromList(std::list<Object *>{&contentStackContainer, &birdContainer});
        mainStack.set_alignment(Center);
        return mainStack;
    }
};

class MyHomeView : public View {
public:
    
    VStack mainStack;
    Text title;
    Container titleContainer = Container(title);
    Image bird;
    Container birdContainer = Container(bird);
    Paragraph description;
    Container descriptionContainer = Container(description);
    Text continueTxt;
    Text exitTxt;
    Button continueBtn = Button(continueTxt);
    Button exitBtn = Button(exitTxt);
    
    MyHomeView(Window & win){
        create(win);
        set_size(900, 600);
        set_alignment(Center);
        fromColorScheme();
    }
    
    void createTitle(Window& window){
        title.from_string("Aves");
        title.set_alignment(Center);
        title.bold(36);
        title.set_foreground_color(Colors::Orange);
        titleContainer.set_size(600, 40);
        titleContainer.disable_reloading();
    }
    
    void createBird(Window& window){
        bird.fromFile("lanUI.Bundle/System/Resources/BlueBird.png", window.sdlRenderer.get());
        window.sdlRenderer.leave();
        bird.set_size(256, 256);
        bird.set_alignment(Center);
        birdContainer.set_size(600, 256);
        birdContainer.disable_reloading();
    }
    
    void createDescription(Window& window){
        std::stringstream stream;
        
        stream << "%color:rgb(0,0,0) %size:14 Uma aplicação feita usando o %color:rgb(0,50,255) framework LANUI %regular %color:rgb(0,0,0) em C++. %newln As informações contidas neste aplicativo foram coletadas no site da %bo %color:rgb(100,100,100) Wiki %ns %color:rgb(0,0,0) pédia. %regular";
        
        description.from_stringstream(stream, Paragraph::Wrapper::Char, 60);
        description.set_alignment(Center);
        descriptionContainer.set_size(600, 80);
        descriptionContainer.disable_reloading();
    }
    
    void createButtons(Window& window){
        continueTxt.from_string("Continuar");
        continueTxt.set_foreground_color(Colors::Orange);
        continueTxt.bold(14);
        continueTxt.set_alignment(Center);
        continueBtn.Object::set_size(600, 20);
        continueBtn.disable_reloading();
        continueBtn.set_padding({5,5});
        
        exitTxt.from_string("Sair");
        exitTxt.set_foreground_color(Colors::Orange);
        exitTxt.bold(14);
        exitTxt.set_alignment(Center);
        exitBtn.Object::set_size(600, 20);
        exitBtn.disable_reloading();
        exitBtn.set_padding({5,5});
        
        exitBtn.on_click(CallbackExpr(
                                      window.shouldClose.set(true);
                                      )
                         );
    }
    
    Object& body(Window& window) override {
        createTitle(window);
        createBird(window);
        createDescription(window);
        createButtons(window);
        mainStack.fromList(std::list<Object *>{&titleContainer, &birdContainer, &descriptionContainer, &continueBtn, &exitBtn});
        mainStack.set_alignment(Center);
        return mainStack;
    }
};

/*
 Animation -> flow animation (controller, sprites) -> destination ? (replace : stop)
 */

int main(int argc, const char * argv[]) {
    // insert code here...
    Core lanUI;
    Window window("Aves", 1100, 600, Window::HighDefinition);
    
    auto Home = MyHomeView(window);
    
    auto Pinguim = BirdView(window, "Pinguim", "O pinguim é uma ave da família %bold Spheniscidae, %regular altamente modificada para a vida aquática, sendo suas asas adaptadas para promover impulso através da água. Estas aves estão amplamente distribuídas pelas águas mais frias do hemisfério sul, especialmente na Antártida e ilhas dos mares austrais, chegado à Terra do Fogo, Ilhas Malvinas e África do Sul.", "lanUI.Bundle/System/Resources/Penguin.png", nullptr, nullptr);
    
    auto Pombo = BirdView(window, "Pombo", "%bold Columbidae %regular é uma família de pássaros que consiste em pombos e pombas. É a única família na ordem Columbiformes. Estas são aves de corpo robusto com pescoços curtos e bicos curtos e delgados que em algumas espécies apresentam ceras carnudas. Eles se alimentam principalmente de sementes, frutas e plantas. A família ocorre em todo o mundo, mas a maior variedade está nos reinos Indomalayan Australasian.", "lanUI.Bundle/System/Resources/Pigeon.png", nullptr, nullptr);
    
    auto Galo = BirdView(window, "Galo", "Galo %bold (Gallus gallus) %regular é o macho da galinha, comumente tratado como um animal heráldico. Estes animais, ao longo da história, também foram utilizados num esporte, atualmente ilegal em diversos países, denominado rinha. Um galo juvenil é chamado de frango, galeto ou galispo.", "lanUI.Bundle/System/Resources/Rooster.png", nullptr, nullptr);
    
    auto Corvo = BirdView(window, "Corvo", "%bold Corvus %regular é um gênero amplamente distribuído de aves de médio a grande porte da família Corvidae. O gênero inclui espécies comumente conhecidas como corvos. Possuem ampla distribuição geográfica nas zonas temperadas de todos os continentes, vivendo em bandos com estrutura hierárquica bem definida e formam, geralmente, casais monogâmicos.", "lanUI.Bundle/System/Resources/Corvus.png", nullptr, nullptr);
    
    auto Papagaio = BirdView(window, "Papagaio", "%bold Amazona aestiva %regular (L.), conhecido vulgarmente como papagaio-verdadeiro, ajuruetê, papagaio-grego, ajurujurá, curau, papagaio-comum, papagaio-curau, papagaio-de-fronte-azul, papagaio-boiadeiro, trombeteiro e louro, é uma ave da família Psittacidae. É nativa do Brasil oriental.", "lanUI.Bundle/System/Resources/Macaw.png", nullptr, nullptr);
    
    auto Coruja = BirdView(window, "Coruja", "%bold Estrigiformes %regular são aves da ordem Strigiformes, que inclui aves de rapina, tais como corujas, mochos e murucututu. São caçadoras eficientes, usando sobretudo seus olhos extremamente aguçados e movimentos rápidos. Além disso, são extremamente atentas ao ambiente, têm grande capacidade de girar o pescoço e voar silenciosamente devido a penas especiais muito macias e numerosas que compõem suas asas.", "lanUI.Bundle/System/Resources/Owl.png", nullptr, nullptr);
    
    Coruja.next = &Papagaio;
    Coruja.last = &Home;
    
    Papagaio.next = &Corvo;
    Papagaio.last = &Coruja;
    
    Corvo.last = &Papagaio;
    Corvo.next = &Galo;
    
    Galo.last = &Corvo;
    Galo.next = &Pombo;
    
    Pombo.last = &Galo;
    Pombo.next = &Pinguim;
    
    Pinguim.last = &Pombo;
    
    lanUI.clearCache();
    
    Home.continueBtn.on_click(CallbackExpr(
                                           window.set_view(Coruja);
                                           )
                              );
    
    Coruja.backwardBtn.on_click(CallbackExpr(
                                           window.set_view(Home);
                                           )
                              );
    
    window
        
    .on_start(
              CallbackExpr(
                           // box[0].set_primary_color(Colors::White);
                           std::cout << "window started" << std::endl;
                           // box[0].set_relative_size(1.0/3.0, 1.0/3.0);
                           )
              )
    
    .on_closed(
               CallbackExpr(
                            std::cout << "window closed" << std::endl;
                            lanUI.terminate();
                            //window.hide();
                            )
               )

    .on_mouse_gained(
                     CallbackExpr(
                                  //.set_size({0, 0, 600, 400});
                                  std::cout << "mouse gained" << std::endl;
                                  )
                     )

    .on_mouse_lost(
                   CallbackExpr(
                                //.set_size({0, 0, 700, 500});
                                std::cout << "mouse lost" << std::endl;
                                )
                   )

    .on_resized(
                CallbackExpr(
                             std::cout << "window resized to:\n " << window.size.get().w;
                             window.size.leave();
                             std::cout << ", " << window.size.get().h << std::endl;
                             window.size.leave();
//                             Home.set_relative_size(1, 1, -10, -10);
//                             Home.nextInZ.get()->set_relative_size(0.5, 0.5);
//                             Home.nextInZ.data->nextInZ.get()->set_relative_size(0.5, 0.5);
//                             Home.nextInZ.data->nextInZ.leave();
//                             Home.nextInZ.leave();
                             )
                )

    .on_key_down(
                 CallbackExpr(
                              std::cout << "key down" << std::endl;
                              // box[0].set_primary_color(Colors::fromRGA(255, 255, 255, 127));
                              )
                 )

    .on_key_up(
               CallbackExpr(
                            std::cout << "key up" << std::endl;
                            // box[0].set_primary_color(Colors::White);
                            )
               )

    .on_mouse_button_down(
                          CallbackExpr(
                                       std::cout << "mouse down" << std::endl;
                                       // box[0].primaryColor.set(Colors::White_smoke);
                                       )
                          )
    .on_mouse_button_up(
                        CallbackExpr(
                                     std::cout << "mouse up" << std::endl;
                                     // box[0].primaryColor.set(Colors::White);
                                     )
                        )
    .on_hidden(
               CallbackExpr(
                            std::cout << "hidden" << std::endl;
                            )
               )

    .on_shown(
              CallbackExpr(
                           std::cout << "shown" << std::endl;
                           )
            )

    .on_minimized(
                  CallbackExpr(
                               std::cout << "minimized" << std::endl;
                               )
                  )

    .on_maximized(
                  CallbackExpr(
                               std::cout << "maximized" << std::endl;
                               //window.set_window_clear_color(Colors::Pink);
                               )
                  )
        
    .set_window_clear_color(Colors::White);
    
    //.embedInZ(box[0]);
    
//    Home.compose(window.sdlRenderer.get(), 2);
//    window.sdlRenderer.leave();
//    Home.set_draw_mode(Object::CompositionMode);
//
    Home.compose(window.sdlRenderer.get(), 2);
    window.sdlRenderer.leave();
//    Home.set_draw_mode(Object::CompositionMode);
    Home.export_composition_as_PNG(window.sdlRenderer.get(), "Home.png");
    window.sdlRenderer.leave();

    Core::events();
    
    std::cout << "Hello, World!\n";
    return 0;
}

