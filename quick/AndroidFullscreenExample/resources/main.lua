
--NB: When hiding the bar, the buttons all move downwards because Quick's coordinate
--system has y=0 at the bottom (opposite of C++ IwGx, s3eSurface, etc.)

local btnCount = 0
local x=director.displayWidth/2
local w=director.displayWidth/5*4
local h=director.displayHeight*0.7/6
local font="fonts/Default.fnt"

local status = director:createLabel({x=10, y=director.displayHeight*0.07, w=director.displayWidth-20, vAlignment="bottom", text="", color=color.blue, font=font})

local function fullscreen(target, phase, action, param1, param2)
    if phase == "began" then
        tween:to(target, {xScale = 0.8, yScale=0.8, time=0.2})
        tween:to(target, {xScale = 1, yScale=1, time=0.2, delay=0.2})
        if action == "on" then
            androidFullscreen.turnOn(param1, param2)
        elseif action == "off" then
            androidFullscreen.turnOff(param1, param2)
        else
            if androidFullscreen.isAvailable() == true then
                if androidFullscreen.isImmersiveSupported() == true then
                    status.text = "fully supported"
                else
                    status.text = "only non-immersive mode is supported"
                end
	        else
    	        status.text =  "not supported at all"
            end
        end
    end
end

local function touchStickyOn(event)
    fullscreen(event.target, event.phase, "on", true, true)
end

local function touchNonStickyOn(event)
    fullscreen(event.target, event.phase, "on", true, false)
end

local function touchNonImmersiveOn(event)
    fullscreen(event.target, event.phase, "on", false, false)
end

local function touchShow(event)
    fullscreen(event.target, event.phase, "off", false, true)
end

local function touchShowBoth(event)
    fullscreen(event.target, event.phase, "off", true, true)
end

local function touchAvailable(event)
    fullscreen(event.target, event.phase, "check")
end

local function addButton(text, touchEvent)
    btnCount = btnCount + 1
    local button = director:createRectangle({x=x, y=director.displayHeight-btnCount*h*1.2, w=w, h=h, color=color.red, xAnchor=0.5, yAnchor=0})
    button:addChild(director:createLabel({x=0, y=0, hAlignment="centre", vAlignment="centre", text=text, w=director.displayWidth/5*4, color=color.white, font=font}))
    button:addEventListener("touch", touchEvent)
end

addButton("Hide nav bar - sticky mode", touchStickyOn)
addButton("Hide nav bar - non-sticky", touchNonStickyOn)
addButton("Hide nav bar - non-immersive", touchNonImmersiveOn)
addButton("Show nav bar", touchShow)
addButton("Show nav & status bars if supported", touchShowBoth)
addButton("Check available", touchAvailable)
