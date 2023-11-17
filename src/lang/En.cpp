#include "En.h"

En::En(QObject *parent)
    : Lang{parent}
{
    setObjectName("En");
    home("Home");
    basic_input("Basic Input");
    form("Form");
    surface("Surfaces");
    popus("Popus");
    navigation("Navigation");
    theming("Theming");
    media("Media");
    dark_mode("Dark Mode");
    sys_dark_mode("Sync with system");
    search("Search");
    about("About");
    settings("Settings");
    locale("Locale");
    navigation_view_display_mode("NavigationView Display Mode");
    other("Other");

    file("File");
    new_("New");
    open("Open...");
    save("Save");
    save_as("Save As...");
    close("Close");
    exit("Exit");
    edit("Edit");
    undo("Undo");
    redo("Redo");
    cut("Cut");
    copy("Copy");
    paste("Paste");
    indent("Indent");
    unindent("Unindent");
    toggle_comment("Toggle Comment");
    search_("Search");
    find("Find...");
    replace("Replace...");
    project("Project");
    new_project_file("New Project File");
    new_class("New Class...");
    new_header("New Header...");
    add_to_project("Add to project...");
    remove_from_project("Remove from project...");
    reformat("Reformat Code");
    execute("Execute");
    compile("Compile");
    run("Run");
    tools("Tools");
    options("Options");
    help("Help");
    about_("About");
    code("Code");
    dir("Choose Folder");

    settings_("Settings");
    theme("Theme");
    system("System");
    light("Light");
    dark("Dark");

    filename("Filename");
    line("Line");
    col("Col");
    description("Description");
    lines("Lines");
}
