function getSyncDataEcmascript(fileName)
{
    return null;
}

var getSyncDataHelper = (this.getSyncDataNative || getSyncDataEcmascript);

function getSyncData(fileName)
{
    return getSyncDataHelper(fileName);
}

function main(databaseName)
{
	var sql = require('D:\\Work\\jssqlapp\\trunk\\jsslapp\\Files\\sql-debug.js')
	var filebuffer = getSyncData(databaseName);
	var db = new sql.Database(filebuffer);
	db.each("SELECT id,url FROM moz_favicons", function(row) {
		console.log(row.id, row.url);
	});
	db.close();
	
}