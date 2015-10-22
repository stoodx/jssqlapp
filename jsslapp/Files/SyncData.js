function getSyncDataEcmascript(fileName)
{
    return null;
}

var getSyncDataHelper = (this.getSyncDataNative || getSyncDataEcmascript);

function getSyncData(fileName)
{
    return getSyncDataHelper(fileName);
}

function main(fileName)
{
    var res = getSyncData(fileName);
    alert(res);
}