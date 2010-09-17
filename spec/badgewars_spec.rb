require File.expand_path('../spec_helper', __FILE__)

describe 'BadgeWars' do
  before :each do
    @world = BadgeWars::World.new
  end
  
  it "has a default world state of 0" do
    BadgeWars.core_size.times do |n|
      @world.peek(n).raw.should == "\0\0\0\0"
      @world[n].raw.should == "\0\0\0\0"
    end
  end
  
  it "can peek and poke the core" do
    @world.peek(0).should be_zero
    @world[0].should be_zero
    
    @world.poke(0, 42)
    @world.peek(0).should == 42
    
    @world[0] = 420
    @world[0].should == 420
  end
  
  it "implements the MOV instruction (i.e. imps work)" do
    @world[0].should be_zero
    @world[1].should be_zero
    
    imp = BadgeWars::Op[:mov, 0, 1]
    @world[0] = imp
    @world.spawn 0
    @world.run
    
    @world[1].should == imp
  end
end