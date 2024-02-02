多语言支持官方手册 squareline

https://docs.squareline.io/docs/multilanguage

github 仓库，其 readme.md 详细说明了使用方法

https://github.com/lvgl/lv_i18n



网上教程文章

https://blog.csdn.net/weixin_43615005/article/details/128954698

https://blog.csdn.net/minorikawa_fuso/article/details/128314627

原理解析

https://zhuanlan.zhihu.com/p/672878099



#### 生成翻译文件使用步骤

nodejs 中文网 http://www.nodejs.com.cn/

npm 教程

- https://www.npmjs.cn/
- https://www.runoob.com/nodejs/nodejs-npm.html
- 简明 https://blog.csdn.net/jieyucx/article/details/131547948

1. 安装 node.js，https://nodejs.org/en/download/

   - 最好下载 bin 便携版 Windows Binary (.zip)（在 C 盘权限受限制的环境用安装版安装不动）。

     放到可联网的环境中，后面通过 npm 在线安装一些依赖包。

     **将 bin 所在目录加入环境变量**（推荐），或者不加之后用绝对路径调用 bin 可执行文件也行。

   - 安装  lv_il8n 包，依赖的包很多，一个个手动解决不现实，最好在线安装 lv_il8n 包：

     （在圈外 / 联网的环境 打开 一个 命令行，下面都是）**执行 `npm i lv_i18n -g`**。

     加入 `-g` 是全局安装，包被安装在 nodejs 安装目录的 node_modules 文件夹里，**将这个文件夹添加系统环境变量**，后面直接 包 直接可以找到，否则就是将包安装到当前目录 node_modules 文件夹里，污染源码，而且还得通过把这个文件夹加入环境变量或者在命令行通过路径找到 lv_i18n 包的路径来执行 lv_i18n 命令。。

     p.s 通过 npm 手动安装包，是 下载 包文件后（文件夹内有 package.json 包信息文件），命令行进入包目录直接执行 `npm install -g`，或者包外面目录执行 `npm i <包路径> -g`。

2. 运行时遇到问题（按照上面步骤一般不会遇到运行问题，缺少的依赖库 npm 会自动解决）：

   - 运行时如遇到问题缺少 es6-error 模块 或者其它模块，同上面一样的 执行在线安装命令。

     离线手动安装 https://github.com/bjyoungblood/es6-error，同上面 手动安装包，但是依赖包很多，这样一个一个下麻烦且不一定下对。

   - 遇到 在此系统上禁止运行脚本 提示，https://blog.csdn.net/fanqiantao/article/details/109650213， powershell 输入 `set-executionpolicy remotesigned` 即可。

3. 运行 lv_i18n 提取所有 当前文件夹下程序文件里 要翻译字符串到 trans 文件夹下 各个语言的 yml 文件里：

   各语言规范编码 language and locale codes：https://www.andiamo.co.uk/resources/iso-language-codes/

   需要预先手动创建各个 `<语言编号>.yml` 文件，并添加一行 `<语言编号>: ~`，如 `en-GB: ~`。

   在工程的顶层目录运行：

   ```bash
   lv_i18n extract -s "./**/*.+(c|cpp|h|hpp)" -t "trans/*.yaml"
   ```

   测试用：

   - 创建 test.c 文件：

     ```c
     lv_obj_t* label1;
     void callback(lv_event_t* e) {
         if (state) {
             lv_i18n_set_locale("en-GB");
         }
         else {
             lv_i18n_set_locale("其他语言");
         }
         lv_label_set_text(label1, _("title1"));
         state = !state;
     }
     
     void lv_example_label_1(void)
     {
         lv_i18n_init(lv_i18n_language_pack);
         lv_i18n_set_locale("en-GB");
         label1 = lv_label_create(lv_scr_act());
         lv_obj_align(label1, LV_ALIGN_CENTER, 0, 100);
         lv_label_set_text(label1, _("title1"));
     	lv_label_set_text(label1, _("aaa"));
     	lv_label_set_text(label1, _("hello	ccc"));
     	lv_label_set_text(label1, _("iii %20"));
     	lv_label_set_text(label1, _("你好~~$^&(* yes"));
         lv_obj_t* btn = lv_obj_create(lv_scr_act());
         lv_obj_align(btn, LV_ALIGN_CENTER, 0, 0);
         lv_obj_set_size(btn,50,50);
         lv_obj_set_style_bg_color(btn, lv_color_hex(0x335566), 0);
         lv_obj_add_event_cb(btn, callback, LV_EVENT_CLICKED, NULL);
     }
     ```

   - 创建 en-GB.yml 文件：

     ```
     en-GB: ~
     ```

   - 在当下目录执行：

     ```bash
     lv_i18n extract -s ./test.c -t ./*.yaml
     ```

   - 即把程序 test.c 中要翻译的字符串提取到了 en-GB.yaml 文件里

4. 在各个语言的 yml 文件里面添加 翻译后的文案，这是手动添加。

   这一步，就可以把 各语言的 *.yaml 传到 localazy 进行翻译，之后 localazy 可以导出 `Rails i18nYAML` 格式的各语言文案文件，再进行下面 `lv_i18n compile ...` 的步骤 将文件 编译为 lv_i18n.c / .h 文件。

   

   对于上面的测试文件，执行 extract 后得到的结果是：

   ```yaml
   en-GB:
     title1: ~
     aaa: ~
     "hello\tccc": ~
     iii %20: ~
     你好~~$^&(* yes: ~
   ```

   将其中的 `~` 替换为 翻译后的文本即可~

5. 在工程的顶层目录，运行 lv_i18n 将 trans 文件夹下的 各个语言的 yml 文件编译成 .c .h 文件 放到 trans 文件夹下，以供 程序使用。

   trans 文件夹需要预先手动创建。

   ```bash
   lv_i18n compile -t "trans/*.yaml" -o "trans"
   ```

   测试用：

   ```bash
   lv_i18n compile -t './*.yaml' -o "trans"
   ```

6. lv_i18n 的 c 程序 api 在 lv_i18n 文件夹下的 .h 文件里，调用即可。

   ```c
   #include "lv_i18n.h"
   
   int32_t ret = lv_i18n_init(lv_i18n_language_pack);
   if(!ret) { ... }
   ret = lv_i18n_set_locale("en-GB");
   if(!ret) { ... }
   
   ...
   
   ret = lv_i18n_set_locale("<其它语言编号>");
   if(!ret) { ... }
   
   ...
   这里就需要刷新所有 label
   ```



其中上面的程序中 "..." 部分，**是要更新所有要翻译的 label**，即手动调一遍所有需要翻译的 label 来更新如：`lv_label_set_text(ui_Label1, _("nozzle temp"));`...这一步或可以脚本完成。

或者后续 squareLine 更新：https://forum.squareline.io/t/for-multilingual-i-hope-squareline-can-generate-a-refresh-function-based-on-the-screen-that-includes-all-the-labels-that-need-to-be-translated/2437



上面的 lv_i18n 的 extract 和 compile 步骤已经写成脚本：

- lv_i18n_extractTrans2yaml.bat
- lv_i18n_compileYaml2Src.bat
