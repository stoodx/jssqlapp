# jssqlapp
Native access to SQLite database via MSVC and Duktape


****Usage**** 

jssqlapp.exe <path_to_start_js_module> <path_to_sql_js_module>  <path_to_sql_database>

For example, start from Debug project:
 
jssqlapp.exe ".\Files\SyncData.js" "D:\Work\jssqlapp\trunk\jsslapp\Files\sql-debug.js"  ".\Files\places.sqlite"

Result:


****
The folder \trunk\jsslapp\src-separate is used only for debug.
