local numbermap = {["2"] = 2, ["1"] = 1, ["0"] = 0, ["-"] = -1, ["="] = -2}
local snafumap = {"=", "-", "0", "1", "2"}

function fromsnafu(snafu)
  local number = 0
  for i = 1, #snafu do
    local char = snafu:sub(#snafu - i + 1, #snafu - i + 1)
    number = number + numbermap[char] * 5^(i - 1)
  end
  return number
end

function tosnafu(number)
  local snafu = {}
  while number > 0 do
    local inc = number + 2
    number = inc // 5
    table.insert(snafu, snafumap[inc % 5 + 1])
  end
  return table.concat(snafu, ""):reverse()
end

local result = 0

while true do
  local line = io.read()
  if line == nil then break end
  result = result + fromsnafu(line:gsub("%s+", ""))
end

print(tosnafu(result))
