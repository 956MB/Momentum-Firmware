#include "../momentum_app.h"

enum VarItemListIndex {
    VarItemListIndexScrollType,
};

void momentum_app_scene_interface_general_var_item_list_callback(void* context, uint32_t index) {
    MomentumApp* app = context;
    view_dispatcher_send_custom_event(app->view_dispatcher, index);
}

static void momentum_app_scene_interface_general_scroll_marquee_changed(VariableItem* item) {
    MomentumApp* app = variable_item_get_context(item);
    bool value = variable_item_get_current_value_index(item);
    variable_item_set_current_value_text(item, value ? "Marquee" : "Standard");
    momentum_settings.scroll_marquee = value;
    app->save_settings = true;
}

void momentum_app_scene_interface_general_on_enter(void* context) {
    MomentumApp* app = context;
    VariableItemList* var_item_list = app->var_item_list;
    VariableItem* item;

    item = variable_item_list_add(
        var_item_list,
        "Text Scroll",
        2,
        momentum_app_scene_interface_general_scroll_marquee_changed,
        app);
    variable_item_set_current_value_index(item, momentum_settings.scroll_marquee);
    variable_item_set_current_value_text(
        item, momentum_settings.scroll_marquee ? "Marquee" : "Standard");

    variable_item_list_set_enter_callback(
        var_item_list, momentum_app_scene_interface_general_var_item_list_callback, app);

    variable_item_list_set_selected_item(
        var_item_list,
        scene_manager_get_scene_state(app->scene_manager, MomentumAppSceneInterfaceGeneral));

    view_dispatcher_switch_to_view(app->view_dispatcher, MomentumAppViewVarItemList);
}

bool momentum_app_scene_interface_general_on_event(void* context, SceneManagerEvent event) {
    MomentumApp* app = context;
    bool consumed = false;

    if(event.type == SceneManagerEventTypeCustom) {
        scene_manager_set_scene_state(
            app->scene_manager, MomentumAppSceneInterfaceGeneral, event.event);
        consumed = true;
        switch(event.event) {
        case VarItemListIndexScrollType:
            scene_manager_next_scene(app->scene_manager, MomentumAppSceneInterfaceGeneral);
        default:
            break;
        }
    }

    return consumed;
}

void momentum_app_scene_interface_general_on_exit(void* context) {
    MomentumApp* app = context;
    variable_item_list_reset(app->var_item_list);
}
