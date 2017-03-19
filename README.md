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

 Block                                       | Value              | Parameters                                                                    | Default
---------------------------------------------|--------------------|-------------------------------------------------------------------------------|-----------------------------------------------------------
 <code style="white-space: pre">%{E}</code>  | Battery Percentage | name, warn (percentage low)                                                   | <code style="white-space: pre">%{EBAT0:12}</code>
 <code style="white-space: pre">%{C}</code>  | CPU Percentage     | index (-1 means all CPUs), warn (percentage high)                             | <code style="white-space: pre">%{C-1:101}</code>
 <code style="white-space: pre">%{M}</code>  | Memory Usage       | type ('U' = used, 'A' = available, 'T' = total), warn (percentage high)       | <code style="white-space: pre">%{MU:0.85}</code>
 <code style="white-space: pre">%{P}</code>  | Temperature        | name, warn (value high)                                                       | <code style="white-space: pre">%{Pthermal_zone0:-1}</code>
 <code style="white-space: pre">%{D}</code>  | Time and Date      | timezone, format (strftime format)                                            | <code style="white-space: pre">%{D:%R}</code>
 <code style="white-space: pre">%{V}</code>  | Time and Date      | card, simple element                                                          | <code style="white-space: pre">%{Vdefault:Master}</code>
 <code style="white-space: pre">%{N}</code>  | Window Title       | monitor                                                                       | <code style="white-space: pre">%{N}</code>
 <code style="white-space: pre">%{L}</code>  | Wireless Network   | interface                                                                     | <code style="white-space: pre">%{Lwlan0}</code>
 <code style="white-space: pre">%{W}</code>  | bspwm Workspaces   | type ('T' = active, 'A' = all, 'U' = current, 'C' = count, separator, monitor | <code style="white-space: pre">%{WT::}</code>
