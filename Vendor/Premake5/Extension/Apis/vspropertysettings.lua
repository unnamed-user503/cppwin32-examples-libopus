require('vstudio')

premake.api.register {
	name  = "vspropertysettings",
	scope = "project",
	kind  = "table",
}

local function importPropertySettings(prj)
    if prj.vspropertysettings then
        for key, value in pairs(prj.vspropertysettings) do
            premake.w("<%s>%s</%s>", key, value, key)
        end
    end
end

premake.override(premake.vstudio.vc2010.elements, "globals", function(oldfn, prj)
    local calls = oldfn(prj)
    table.insertafter(calls, premake.vstudio.vc2010.globals, importPropertySettings)
    return calls
end)
