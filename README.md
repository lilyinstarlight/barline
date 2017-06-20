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

 Block   | Value              | Parameters                                                                     | Default
---------|--------------------|--------------------------------------------------------------------------------|-----------------------
 `%{E}`  | Battery Percentage | name, warn (percentage low)                                                    | `%{EBAT0:12}`
 `%{C}`  | CPU Percentage     | index (-1 means all CPUs), warn (percentage high)                              | `%{C-1:101}`
 `%{M}`  | Memory Usage       | type ('U' = used, 'A' = available, 'T' = total), warn (percentage high)        | `%{MU:0.85}`
 `%{P}`  | Temperature        | name, warn (value high)                                                        | `%{Pthermal_zone0:-1}`
 `%{D}`  | Time and Date      | timezone (empty for system setting), format (strftime format)                  | `%{D:%R}`
 `%{V}`  | Volume             | card, simple element                                                           | `%{Vdefault:Master}`
 `%{N}`  | Window Title       | monitor                                                                        | `%{N}`
 `%{L}`  | Wireless Network   | interface                                                                      | `%{Lwlan0}`
 `%{W}`  | bspwm Workspaces   | type ('T' = active, 'A' = all, 'U' = current, 'C' = count), separator, monitor | `%{WT::}`
