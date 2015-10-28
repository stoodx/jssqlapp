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
	//	var filebuffer = readFile(databaseName);
	/*	var db = new sql.Database(filebuffer);
		db.each("SELECT id,url FROM moz_favicons", function(row) {
			console.log(row.id, row.url);
		});
		db.close();*/
		print("Test finish");
	}	catch (e) {
		print(e.stack);
	}
}