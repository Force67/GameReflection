-- Copyright (C) Force67 2021.
-- For licensing information see LICENSE at the root of this distribution.

local p = premake

function dump_all_workspaces()
    for wks in p.global.eachWorkspace() do
        for prj in p.workspace.eachproject(wks) do
            print(prj.name)
        end
    end
end

function dumpfiles(wks)
    for prj in p.workspace.eachproject(wks) do
        print(prj.name)
    end
end