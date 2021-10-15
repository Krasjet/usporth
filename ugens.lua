-- ugens.lua: description of unit generators,
--            also generates ugens.h

local ugens = {
  ['+'] = { alias = 'add' },
  ['-'] = { alias = 'sub' },
  ['*'] = { alias = 'mul' },
  ['/'] = { alias = 'div' },

  ['add'] = {
    input = {
      {name = 'v1', type = 'f'},
      {name = 'v2', type = 'f'},
    },
    output = {
      {name = 'sum', type = 'f'},
    },
    description = 'add two numbers (v1 + v2)'
  },

  ['sub'] = {
    input = {
      {name = 'v1', type = 'f'},
      {name = 'v2', type = 'f'},
    },
    output = {
      {name = 'diff', type = 'f'},
    },
    description = 'subtract two numbers (v1 - v2)'
  },

  ['mul'] = {
    input = {
      {name = 'v1', type = 'f'},
      {name = 'v2', type = 'f'},
    },
    output = {
      {name = 'prod', type = 'f'},
    },
    description = 'multiply two numbers (v1 * v2)'
  },

  ['div'] = {
    input = {
      {name = 'v1', type = 'f'},
      {name = 'v2', type = 'f', cond = 'x != 0'},
    },
    output = {
      {name = 'quot', type = 'f'},
    },
    description = 'divide two numbers (v1 / v2)'
  },

  ['inv'] = {
    input = {
      {name = 'v', type = 'f', cond = 'x != 0'},
    },
    output = {
      {name = 'inv', type = 'f'},
    },
    description = 'compute 1 / v'
  },

  ['swap'] = {
    input = {
      {name = 'v1', type = 'f'},
      {name = 'v2', type = 'f'},
    },
    output = {
      {name = 'v2', type = 'f'},
      {name = 'v1', type = 'f'},
    },
    description = 'move the second item to top (1 2 swap => 2 1)'
  },

  ['rot'] = {
    input = {
      {name = 'v1', type = 'f'},
      {name = 'v2', type = 'f'},
      {name = 'v3', type = 'f'},
    },
    output = {
      {name = 'v2', type = 'f'},
      {name = 'v3', type = 'f'},
      {name = 'v1', type = 'f'},
    },
    description = 'move the third item to top (1 2 3 rot => 2 3 1)'
  },

  ['drop'] = {
    input = {
      {name = 'v'},
    },
    output = {},
    description = 'remove the last item from stack (1 2 drop => 1)'
  },

  ['dup'] = {
    input = {
      {name = 'v', type = 'f'},
    },
    output = {
      {name = 'v', type = 'f'},
      {name = 'v', type = 'f'},
    },
    description = 'duplicate the last item (1 dup => 1 1)'
  },

  ['dup2'] = {
    input = {
      {name = 'v1', type = 'f'},
      {name = 'v2', type = 'f'},
    },
    output = {
      {name = 'v1', type = 'f'},
      {name = 'v2', type = 'f'},
      {name = 'v1', type = 'f'},
      {name = 'v2', type = 'f'},
    },
    description = 'duplicate the last two items (1 2 dup2 => 1 2 1 2). '..
                  'useful for stereo signal'
  },

  ['in'] = {
    input = {},
    output = {
      {name = 'sample', type = 'f'},
    },
    description = 'push input sample to stack'
  },

  ['sine'] = {
    input = {
      {name = 'freq', type = 'f'},
    },
    output = {
      {name = 'sample', type = 'f'},
    },
    description = 'sine oscillator'
  },

  ['scale'] = {
    input = {
      {name = 'val', type = 'f'},
      {name = 'min', type = 'f', cond = 'x != max'},
      {name = 'max', type = 'f', cond = 'x != min'},
    },
    output = {
      {name = 'sample', type = 'f'},
    },
    description = 'scale unipolar input value from [0,1] to [min,max]'
  },

  ['biscale'] = {
    input = {
      {name = 'val', type = 'f'},
      {name = 'min', type = 'f', cond = 'x != max'},
      {name = 'max', type = 'f', cond = 'x != min'},
    },
    output = {
      {name = 'sample', type = 'f'},
    },
    description = 'scale bipolar input value from [-1,1] to [min,max]'
  },

  ['metro'] = {
    input = {
      {name = 'freq', type = 'f', cond = '0 <= x <= sr'},
    },
    output = {
      {name = 'trigger', type = 'f'},
    },
    description = 'generate clock signal (impulse train) from frequency'
  },

  ['dmetro'] = {
    input = {
      {name = 'duration (sec)', type = 'f', cond = 'x >= 0'},
    },
    output = {
      {name = 'trigger', type = 'f'},
    },
    description = 'generate equally spaced clock signal (impulse train) from duration'
  },

  ['tgate'] = {
    input = {
      {name = 'trigger', type = 'f'},
      {name = 'time (sec)', type = 'f', cond = 'x >= 0'},
    },
    output = {
      {name = 'gate', type = 'f'},
    },
    description = 'generate gate signal for `time` seconds on trigger'
  },

  ['adsr'] = {
    input = {
      {name = 'gate', type = 'f'},
      {name = 'attack (sec)', type = 'f', cond = 'x >= 0'},
      {name = 'decay (sec)', type = 'f', cond = 'x >= 0'},
      {name = 'sustain', type = 'f', cond = '0 <= x <= 1'},
      {name = 'release (sec)', type = 'f', cond = 'x >= 0'},
    },
    output = {
      {name = 'envelope', type = 'f'},
    },
    description = 'generate exponential adsr envelope from gate signal'
  },

  ['ftgen'] = {
    input = {
      {name = 'ft name', type = 's'},
      {name = 'vals', type = 's'},
    },
    output = { },
    description = 'generate function table with space-delimited values'
  },

  ['ftget'] = {
    input = {
      {name = 'index', type = 'f', cond = '0 <= x < ftlen'},
      {name = 'ft name', type = 's'},
    },
    output = {
      {name = 'val', type = 'f'},
    },
    description = 'read from table at index (no interpolation)'
  },

  ['ftset'] = {
    input = {
      {name = 'val', type = 'f'},
      {name = 'index', type = 'f', cond = '0 <= x < ftlen'},
      {name = 'ft name', type = 's'},
    },
    output = { },
    description = 'set table value at index'
  },

  ['tseq'] = {
    input = {
      {name = 'trigger', type = 'f'},
      {name = 'ft name', type = 's'},
    },
    output = {
      {name = 'sequence', type = 'f'},
    },
    description = 'trigger controlled sequencer'
  },

  ['mtof'] = {
    input = {
      {name = 'midi', type = 'f'},
    },
    output = {
      {name = 'freq', type = 'f'},
    },
    description = 'convert midi note number to frequency'
  },

  ['set'] = {
    input = {
      {name = 'val', type = 'f'},
      {name = 'var name', type = 's'},
    },
    output = { },
    description = 'set variable to val, or create new variable if not exist'
  },

  ['get'] = {
    input = {
      {name = 'var name', type = 's'},
    },
    output = {
      {name = 'val', type = 'f'},
    },
    description = 'get variable value'
  },


  -- add more ugens here
}

-- generate definition from description
local function mkdef(keyword, alias)
  local nfunc = alias and alias or keyword
  local nenum = alias and
    "SYM_" .. string.upper(alias) or
    string.upper(keyword)
  return string.format(
    [[USPORTH_UGEN("%s", UGEN_%s, ugen_%s_init, ugen_%s_tick, ugen_%s_free)]],
    keyword, nenum , nfunc, nfunc, nfunc
  )
end

-- sort ugens by name
local sorted_keys = {}
for k in pairs(ugens) do
  table.insert(sorted_keys, k)
end
table.sort(sorted_keys)

-- print ugens.h header
print("/* ugens.h: definition for unit generators, generated by ugens.lua */")
for _, k in ipairs(sorted_keys) do
  print(mkdef(k, ugens[k].alias))
end
