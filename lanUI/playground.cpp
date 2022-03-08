//
//  playground.cpp
//  lanUI
//
//  Created by René Descartes Domingos Muala on 27/08/21.
//  Copyright © 2021 René Descartes Domingos Muala. All rights reserved.
//

#include "lanUI.hpp"
#include <regex>

#include "Interface/TextField/TextField.hpp"

/*
 Note:
    On Xcode, it will be necessary to disable metal api validation.
    Otherwise your application will immediately crash after resizing the window.
    (Disable it on: Scheme > Edit Scheme... > Run > Diagnostics or Options > Metal API Validation).
 */

#include <iostream>

using namespace std;

class MainView : public View {
    Paragraph question;
    List answers;
    HStack mainstask;
    Text * count;
    Loader loader;
    int counter=0;
public:
    MainView(Window & win){
        create(win).set_alignment(Center);
        set_size(600, 400);
    }
    
    Object& body(Window & win) override {
        question.from_string("%Size:13 %Medium Baixando %[count] 0 mb(s)");
        if((count = question["count"])){
            count->set_default_animation(120, CallbackExpr({
                counter++;
                loader.state.hold();
                if(counter%5 ==0)
                    loader.state.data = (loader.state.data == BSLoader::Indeterminate) ? BSLoader::Active : BSLoader::Indeterminate;
                loader.state.leave();
                count->from_string(std::to_string(counter));
                return true;
            }));
        }
        
//        loader.set_state(BSLoader::Disabled);
        loader.set_radius(50);
        loader.set_padding(2);
        loader.set_weight(300);
//        loader.set_size(100, 50);
        loader.set_foreground_color(Colors::Blue_violet);
        loader.set_background_color(Colors::Light_gray);
        mainstask.fromList((std::list<Object *>){&loader, &question});
        mainstask.set_alignment(Center);
        return mainstask;
    }
};

#include <queue>

class MyMask : public Mask {
public:
    void apply() override{
        for(int i = 0 ; i <= 40 ; i++){
            for(int j = 0 ; j <= 40 ; j++){
                cairo_rectangle(context, (i/40.0)-0.005, (j/40.0)-0.005, 0.02, 0.02);
            }
        } cairo_fill(context);
    }
};

class GradMask : public Mask {
public:
    void apply() override{
        cairo_pattern_t * grad = cairo_pattern_create_linear(0.5, 0, 0.5, 1);
        cairo_pattern_add_color_stop_rgba(grad, 0, 1, 1, 1, 0);
        cairo_pattern_add_color_stop_rgba(grad, 0.5, 1, 1, 1, 1);
        cairo_pattern_add_color_stop_rgba(grad, 1, 1, 1, 1, 0);
        cairo_mask(context, grad);
    }
};

class CircleMask : public Mask {
public:
    void apply() override{
        cairo_pattern_t * grad = cairo_pattern_create_radial(0.5, 0.5, 0.4, 0.50, 0.5, 0.5);
        cairo_pattern_add_color_stop_rgba(grad, 0.99, 1, 1, 1, 1);
        cairo_pattern_add_color_stop_rgba(grad, 1, 0, 1, 1, 0);
        cairo_mask(context, grad);
    }
};


class QuadTest : public Object {
public:
    void default_composer_callback() {
        CairoContext * cr = cairoContext;
        
        cairo_set_source_rgb(cr, 0, 0, 0);
        
        cairo_move_to(cr, 0.5, 0.25);
        cairo_line_to(cr, 0.75, 0.75);
        cairo_line_to(cr, 0.25, 0.75);
        cairo_close_path(cr);
        
        cairo_set_line_width(cr, 0.001);
        
        cairo_stroke(cr);
    }
    
    QuadTest(){
        Object();
        set_composer_callback(CallbackExpr(default_composer_callback();));
    }
    
    void _compose(Renderer * renderer, const float dpiK, const int32_t windowId) override {
        if(wasCompiled.get())
            wasCompiled.data = dpiK == obj_dpiK.get_copy();
        wasCompiled.leave();
        obj_dpiK.set(dpiK);
        if(!wasCompiled.get()){
            _composer_routine(renderer);
//            wasCompiled.data = true;
        } wasCompiled.leave();
        _compose_embedded(renderer, dpiK, windowId);
    }
};

namespace WMSSharedData
{
extern std::vector<Window *> programWindows;
}

class TextTest : public Object {
public:
    FontFace ftface;
    Font font = Fonts::WorkSans;
    CairoFontFace * cftface;
    CairoFontExtents fe;
    CairoTextExtents te;
    const double size = 46;
    const char * source = "int a[9] = {1,2,3,4,5,6};";
    void default_composer_callback() {
//        font.set_style(Font::Bold);
        
        CairoContext * cr = composerParams.context;
        
        cairo_set_source_rgb(cr, 0, 0, 0);
        
        cairo_set_font_face(cr, cftface);
        
        cairo_set_font_size(cr, size/composerParams.greaterSide);
       /*
        cairo_font_extents(cr, &fe);
        
        cairo_text_extents(cr, source, &te);
        */
        cairo_move_to(cr, -te.x_bearing/composerParams.greaterSide, fe.ascent/composerParams.greaterSide);
        
        cairo_show_text(cr, source);
    }
    
    void _compose(Renderer * renderer, const float dpiK, const int32_t windowId) override {
        if(wasCompiled.get())
            wasCompiled.data = dpiK == obj_dpiK.get_copy();
        wasCompiled.leave();
        obj_dpiK.set(dpiK);
        
        TextParams * tp = &WMSSharedData::programWindows[windowId]->textParams;
        
        
        if(!wasCompiled.get()){
            wasCompiled.leave();
            font.generate_ft_face(ftface, 1);
            cftface = cairo_ft_font_face_create_for_ft_face(ftface.get_face(), 0);
            tp->compute_params(cftface, te, fe, size, source);
            cout << te.x_advance << " " << fe.height << " " << size << " "<< source << std::endl;
            set_size(te.x_advance, fe.height);
            _composer_routine(renderer);
            ftface.done_face();
            cairo_font_face_destroy(cftface);
//            wasCompiled.data = true;
        } wasCompiled.leave();
        _compose_embedded(renderer, dpiK, windowId);
    }
    
    TextTest(double size): size(size){
        Object();
        this->font.set_size(12);
        set_composer_callback(CallbackExpr(default_composer_callback();));
    }
};

int main() {
//   Core program;
    
    Window window("LUI", 600, 500, Window::HighDefinition);
    
    window.set_window_clear_color(Colors::White_smoke);
    
    Image paisagem;
    
    paisagem.set_size(400, 400);
    
    paisagem.set_alignment(Object::Center);
    
    paisagem.set_composer_callback(CallbackExpr({
        
        cairo_arc(paisagem.cairoContext, 0.5, 0.5, 0.5, 0, 360.0*(M_PI/180.0));
        
        cairo_set_source_rgb(paisagem.cairoContext, 0.96, 0.48  , 0.18);
        
        cairo_fill(paisagem.cairoContext);
        
        cairo_set_line_width(paisagem.cairoContext, paisagem.composer_get_cairo_pixel_size() * 10);

        cairo_set_source_rgb(paisagem.cairoContext, 1,1,1);
        
        cairo_curve_to(paisagem.cairoContext, 0.5, 0.8, 0.5, 0.5, 0.25, 0.35);
        
        cairo_stroke(paisagem.cairoContext);
        
        cairo_curve_to(paisagem.cairoContext, 0.5, 0.8, 0.5, 0.5, 0.5 + 0.25, 0.35);
        
        cairo_stroke(paisagem.cairoContext);
        
        cairo_curve_to(paisagem.cairoContext, 0.5, 0.8, 0.5, 0.5, 0.5, 0.25);

        cairo_stroke(paisagem.cairoContext);
        
        cairo_set_source_rgb(paisagem.cairoContext, 0.31, 0.56, 0.95);
        
        cairo_arc(paisagem.cairoContext, 0.25, 0.35, 0.08, 0, 360.0*(M_PI/180.0));
        
        cairo_fill(paisagem.cairoContext);
        
        cairo_arc(paisagem.cairoContext, 0.5 + 0.25, 0.35, 0.08, 0, 360.0*(M_PI/180.0));
        
        cairo_fill(paisagem.cairoContext);
        
        cairo_arc(paisagem.cairoContext, 0.5, 0.25, 0.08, 0, 360.0*(M_PI/180.0));
        
        cairo_fill(paisagem.cairoContext);

    }));
    
//    Text example("{0} Á {0}", Fonts::DejaVuSans), example2("{1} Á {1}", Fonts::DejaVuSans);
    
//    Paragraph example;

//    example.from_string("%size:156 %font:Dejavusans %color:rgb(0,0,0) %ns 0 %ns 0 %ns 0 %ns 0 %s");

    TextTest example(20), example2(40);
    
    example.set_size_compensation(0, 0);
    
    window.embedInZ(example);
    
    /*
    example.set_font_size(20);

    example2.set_font_size(40);
    
    example.set_foreground_color(Colors::Black.from_a(250));
    example2.set_foreground_color(Colors::Black.from_a(250));
*/
    example.embedInY(example2);

    example.set_alignment(Object::Center);
    
//    example.set_font(Fonts::OpenSans);
        
//    example.composer_push_mask(new MyMask());
        
    /*
    cairo_pattern_t * pattern = cairo_pattern_create_linear(0, 0, 1, 0.5);
    cairo_pattern_add_color_stop_rgba(pattern, 0.0, 1, 0, 0, 1);
    cairo_pattern_add_color_stop_rgba(pattern, 0.5, 0, 1, 0, 1);
    cairo_pattern_add_color_stop_rgba(pattern, 1.0, 0, 0, 1, 1);
    
    example.set_foreground_pattern(pattern);
    */
    return LUI::run();
}
