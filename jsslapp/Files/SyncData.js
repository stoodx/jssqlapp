function getError(fileName)
{
	print("getError: Not init");
    return null;
}

var readFileHelper = (this.readFileNative || getError);

function readFile(fileName)
{
    return readFileHelper(fileName);
}

Duktape.modSearch = function (id) 
{//require
    print('Loading module: ', id);
    // Return source code for module or throw an error.
	var res = readFile(id);
    if (typeof res === 'buffer') 
	{	
		print('require() - ok');
		return res.toString('ascii');
    }
    throw new Error('module not found: ' + id);	
};

function main(sqlModule, databaseName)
{
	try {	
		var sql = require(sqlModule);

		print("readFile")
		var filebuffer = readFile(databaseName);

		print("sql.Database")
		var db = new sql.Database(filebuffer);

		print("start SELECT")
		db.each("SELECT id,url FROM moz_favicons", function(row) {
			print(row.id, row.url);
		});

		print("db.close")
		db.close();
		
		print("Test finish");
	}	catch (e) {
		print(e.stack);
	}
}