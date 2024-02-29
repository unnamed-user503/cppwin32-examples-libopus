require('vstudio')

premake.api.register {
	name  = "vsadditionalprops",
	scope = "project",
	kind  = "table",
}

local function importAdditionalProps(prj)
    if prj.vsadditionalprops then
        for i, entry in ipairs(prj.vsadditionalprops) do
            premake.w('<Import Project="%s.props" />', entry) 
        end
    end
end

premake.override(premake.vstudio.vc2010.elements, "project", function(oldfn, prj)
    local calls = oldfn(prj)
    table.insertafter(calls, premake.vstudio.vc2010.project, importAdditionalProps)
    return calls
end)
