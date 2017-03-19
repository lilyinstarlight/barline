barline
=======

barline is a small data collector that is designed to function between lemonbar and bspwm. It sources data from bspwm and system data such as cpu usage, memory usage, temperature, etc. It uses a lemonbar like syntax for its format and passes any unknown directive to lemonbar.


Configuration
-------------

barline is configured via the '~/.config/barline/barlinerc' file in your home directory. Below is an example from my 'barlinerc' and further down is a table of supported blocks.


### Example

```barlinerc
format %{l}%{W}%{c}%{N}%{r}cpu: %{C} • temp: %{Pthermal_zone1:60} • mem: %{M} • vol: %{V} • wlan: %{Lwlp8s0} • batt: %{EBAT1} • %{DAmerica/New_York}
poll 1
```


### Blocks

Blocks are delimited by `%{` and `}` and are formatted with the first parameter right after the block type and further parameters separated by `:`. Any number of trailing parameters are optional and are assumed to be the default. Unknown blocks are passed directly to lemonbar.

 Block           | Value              | Parameters                                                                    | Default
-----------------|--------------------|-------------------------------------------------------------------------------|--------------------------------
 <pre>%{E}</pre> | Battery Percentage | name, warn (percentage low)                                                   | <pre>%{EBAT0:12}</pre>
 <pre>%{C}</pre> | CPU Percentage     | index (-1 means all CPUs), warn (percentage high)                             | <pre>%{C-1:101}</pre>
 <pre>%{M}</pre> | Memory Usage       | type ('U' = used, 'A' = available, 'T' = total), warn (percentage high)       | <pre>%{MU:0.85}</pre>
 <pre>%{P}</pre> | Temperature        | name, warn (value high)                                                       | <pre>%{Pthermal_zone0:-1}</pre>
 <pre>%{D}</pre> | Time and Date      | timezone, format (strftime format)                                            | <pre>%{D:%R}</pre>
 <pre>%{V}</pre> | Time and Date      | card, simple element                                                          | <pre>%{Vdefault:Master}</pre>
 <pre>%{N}</pre> | Window Title       | monitor                                                                       | <pre>%{N}</pre>
 <pre>%{L}</pre> | Wireless Network   | interface                                                                     | <pre>%{Lwlan0}</pre>
 <pre>%{W}</pre> | bspwm Workspaces   | type ('T' = active, 'A' = all, 'U' = current, 'C' = count, separator, monitor | <pre>%{WT::}</pre>
