#include "Zh.h"

Zh::Zh(QObject *parent)
    : Lang{parent}
{
    setObjectName("Zh");
    home("首页");
    basic_input("基本输入");
    form("表单");
    surface("表面");
    popus("弹窗");
    navigation("导航");
    theming("主题");
    media("媒体");
    dark_mode("夜间模式");
    sys_dark_mode("跟随系统");
    search("查找");
    about("关于");
    settings("设置");
    locale("语言环境");
    navigation_view_display_mode("导航视图显示模式");
    other("其他");

    file("文件");
    new_("新建");
    open("打开...");
    save("保存");
    save_as("另存为...");
    close("关闭");
    exit("退出");
    edit("编辑");
    undo("撤销");
    redo("重做");
    cut("剪切");
    copy("复制");
    paste("粘贴");
    indent("缩进");
    unindent("取消缩进");
    toggle_comment("切换注释");
    search_("搜索");
    find("查找...");
    replace("替换...");
    project("项目");
    new_project_file("新建项目文件");
    new_class("新建类...");
    new_header("新建头文件...");
    add_to_project("添加至项目...");
    remove_from_project("从项目...移除");
    reformat("重排代码");
    execute("执行");
    compile("编译");
    run("运行");
    tools("工具");
    options("选项");
    help("帮助");
    about_("关于");
    code("代码");
    dir("选择文件夹");

    settings_("设置");
    theme("外观");
    system("跟随系统");
    light("日间模式");
    dark("夜间模式");

    filename("文件名");
    line("行");
    col("列");
    description("信息");
    lines("总行数");
}
