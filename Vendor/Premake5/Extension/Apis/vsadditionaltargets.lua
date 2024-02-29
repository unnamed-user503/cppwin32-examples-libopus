require('vstudio')

premake.api.register {
	name  = "vsadditionaltargets",
	scope = "project",
	kind  = "table",
}

local function importAdditionalTargets(prj)
    if prj.vsadditionaltargets then
        for i, entry in ipairs(prj.vsadditionaltargets) do
            premake.w('<Import Project="%s.targets" />', entry) 
        end
    end
end

premake.override(premake.vstudio.vc2010.elements, "project", function(oldfn, prj)
    local calls = oldfn(prj)
    table.insertafter(calls, premake.vstudio.vc2010.project, importAdditionalTargets)
    return calls
end)
