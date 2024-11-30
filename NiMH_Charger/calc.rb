#!/usr/bin/ruby
# -*- coding: utf-8 -*-

dTja=125.0
Rjc=12.5
Rca=20.0

dTca = dTja/(Rjc/Rca+1.0)
dTjc = dTja-dTca
P = dTca/Rca

puts "dTca="+dTca.to_s+"[K]"
puts "dTjc="+dTjc.to_s+"[K]"
puts "P="+P.to_s+"[W]"

# kenzan
#P1=dTjc/Rjc
#puts "P="+P1.to_s+"[W]"
