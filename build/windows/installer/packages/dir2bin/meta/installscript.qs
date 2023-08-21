function Component()
{
}

Component.prototype.createOperationsForArchive = function(archive)
{
    component.addOperation("Extract", archive, "@TargetDir@/bin");
}

Component.prototype.createOperations = function()
{
    // call default implementation
    component.createOperations();

    component.addOperation(
                  "Execute",
                  '"@TargetDir@\\bin\\postinstall.cmd" "@TargetDir@"',
                  "UNDOEXECUTE",
                  '"@TargetDir@\\bin\\preremove.cmd"');
}
