# Общее описание
Репозиторий с проектами и материалами для видеокурса "Renga. Инструменты автоматизации. Программирование"

## Обращение к пользователям (от автора)
Приветствуем вас на данном репозитории. Это первый мой курс по программированию под САПР и первый в таком ключе - с публикацией материалов не только в видеоформате, но также в текстовом, графическом и с приложением всех наработок в виде программного кода и комментариев к нему.

Организационно, репозиторий построен по следующей схеме: данный файл (корневой ReadMe.md) является начальной страницей, с которого вы будете переходить на иные ресурсы. Часть проектов, которые будут упоминаться в видеокурсе, находятся не здесь, а в других публичных репозиториях (ссылки на всё также будут представлены). На случай блокировки репозиторий или платформы, материал также будет доступен на площадках:
- Блог разработчика Renga [link]();
- ...

## Структура репозитория
```\lessons``` - разбитые по блокам темы, в каждой теме примеры исходного кода (скриптов), демонстировавшихся в видео;

```\templates``` - шаблоны плагинов для Renga с исходным кодом на C#, Visual C++, C++ (QT);

```\docs``` - справочная документация (текстовое поосбие и иные вспомогательные материалы;

# Перечень тем в курсе
Все темы разбиты по уровням восприятия, в справочном пособии этот уровень прописан. 
Обозначение | О чем раздел
--|--
🟢 | Материал для всех, читать обязательно;
🟤 | Факультативный материал, рекомендуется читать, но может показаться сложным;
🟡 | Материал по работе в среде Dynamo;
🟠 | Материал по работе с C#;
🟣 | Материал по работе с C++;

**Блок-1: Введение**
- 🟢 Приветствие слушателей курса, слово от автора курса. Демонстрация программы курса

**Блок-2: Renga API - общая информация. COM. Подключение к процессу Renga**
- 🟢 Справочные ресурсы, где смотреть информацию о Renga API. Renga SDK. Установка SDK и обзор файлов;
- 🟤 Текстовая справка о технологии COM и принципах работы с Renga;

Файл с примерами и ресурсами дял разработки [см. здесь](https://github.com/GeorgGrebenyuk/renga_programming_course_1/blob/main/info_resources.md)

**Блок-3: Создание плагина над программой**;
- 🟢 Техническое видео-презентация: схема создания плагина, инициализация интерфейса, встраивание плагина в программу, контроль ошибок и отладка, настройка конфигураций сборки с копированием файлов проекта, иконки и картинки;
- 🟢 События в Renga. Виды событий (теория);
- 🟠 Создание библиотеки классов на .NET Framework 4.5.2 (C#, VS 2019) с выводом сообщения в диалоговом окне "Привет, Ренга" по нажатию на кнопку, отладка, настройка проекта, формирование шаблона;
- 🟣 Создание библиотеки DLL (Visual C++, VS 2019) с выводом сообщения в диалоговом окне "Привет, Ренга" по нажатию на кнопку, отладка, настройка проекта, формирование шаблона;
- 🟣 Создание библиотеки shaded library (C++, QT Creator 6.0.2) с выводом сообщения в диалоговом окне "Привет, Ренга" по нажатию на кнопку, отладка, настройка проекта;
- 🟠 Реализация примера пользователей "Кнопка возврата выделения группы объектов" -- боевое применение логики событий (фиксация в памяти списка пользовательских выборов с фиксированием времени, по нажатию на копку вывод списка в DataGrid структуры WinForms, по выбору пользователя выбор группы действий, выделение идентификаторов в модели); https://github.com/GeorgGrebenyuk/renga_select_last_objects

[Перейти к разделу с исходным кодом](https://github.com/GeorgGrebenyuk/renga_programming_course_1/blob/main/lessons/blok3/block3_about.md)

[Перейти к разделу с шаблонами](https://github.com/GeorgGrebenyuk/renga_programming_course_1/blob/main/templates/about.md)

**Блок-4: Работа с объектами Renga**
- 🟢🟤 Техническое видео-презентация: как хранятся объекты внутри программы, какие интерфейсы используются для доступа к ним, к свойствам (3 типа), к геометрии, к материалам. Существующие ограничения;
- 🟣 Реализация плагина на Visual C++ по экспорту геометрии и свойств в формат NWC (Autodesk Navisworks); https://github.com/GeorgGrebenyuk/renga_3d-export
- 🟠 Реализация плагина по контролю за пользовательскими действиями в модели по выбору объектов модели с целью запрета выделения отдельных объектов. https://github.com/GeorgGrebenyuk/Renga_FollowUsersActions

[Перейти к разделу с исходным кодом](https://github.com/GeorgGrebenyuk/renga_programming_course_1/blob/main/lessons/blok4/blok4_about.md)

🟡 **Блок-5: Dynamo для Renga**
- Знакомство со средой Dynamo Core. Установка, загрузка пакета для работы с Renga;
- Демонстрация процессов работы с документами, объектами, свойствами; особенность работы с Dynamo Python (CPython);
- Подгрузка OSM-подосновы (зданий) в координатах используя вспомогательные пакеты нодов ```dyn_proj_library```, ```osm2dyn```, ```dyn2ifc```;
- Автоматизация формирования файла маппинга свойств ```export_attr_qto_pset.json``` для экспорта в IFC с любыми пользовательскими свойствами;

**Блок-6: Итоги**
- Демонстрация данного репозитория, знакомство с логикой отправки обратной связи по ошибкам на sd.askon;
- форма пожеланий к нерассмотренным темам и продолжению курса.
