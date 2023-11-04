target("Glad")
    set_kind("static")
    set_languages("c11")

    add_files("src/**.c")
    add_includedirs("include")
