# Требуемый интерфейс базы данных С++ 

При старте программы КСУ-3 считать всю информацию из базы данных в контейнер, map of struct.
Это касается структур параметров, уставок, событий, защит, лейблов.
```python
cls.param_table.collect_all_items()
cls.setting_table.collect_all_items()
cls.event_table.collect_all_items()
cls.prot_table.collect_all_items(type_sb)
cls.label_table.collect_all_items()
```

Важно, быстро получать и записывать значения параметров (возможна группповая запись значений) и уставок в контейнер по строковому id, а также получать значения конкретных полей.
```python
DB.setting_table.get_value("id_setting")
DB.param_table.get_value("id_param")
# get_record_id(db_id)
# get_name(db_id)
# get_unit(db_id)
# get_dot(db_id)
DB.setting_table.set_value("id_setting", value, ChangeType.REMOTE)
```

Изменения значений уставок своеврменно фиксировать в базе данных.
Быстрый поиск дерева заголовков меню:
 ```python
DB.menu_header_table.get_main_headers()
DB.menu_header_table.get_sublevel_headers(main_level)
```

Быстрый поиск дерева уставок/параметров по 1 и 2 уровням:
 ```python
DB.setting_table.get_menu_item_list(main_level, second_level)
DB.param_table.get_menu_item_list(main_level, second_level)
```

Замерить, как быстро С++ сможет сформировать JSON-объект значений всех уставок/параметров в порядке id базы данных или строковых db_id (главное, чтобы порядок не изменялся как это происходит в Python), рассчитать CRC32 и закодировать base64 или зашифровать для сравнения с Python.