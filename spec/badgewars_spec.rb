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
    @world.peek(0).raw.should == "\0\0\0\0"
    @world[0].raw.should == "\0\0\0\0"
    
    @world.poke(0, BadgeWars::Op[:mov, 42, 420])
    @world.peek(0).should be_eql(BadgeWars::Op[:mov, 42, 420])
    
    @world[0] = BadgeWars::Op[:mov, 42, 420]
    @world[0].should be_eql(BadgeWars::Op[:mov, 42, 420])
  end
  
  it "implements the MOV instruction (i.e. imps work)" do
    @world[0].raw.should == "\0\0\0\0"
    @world[1].raw.should == "\0\0\0\0"
    
    imp = BadgeWars::Op[:mov, 0, 1]
    @world[0] = imp
    @world.spawn 0
    @world.run
    
    @world[1].should be_eql(imp)
    @world[2].raw.should == "\0\0\0\0"
    @world.run
    
    @world[2].should be_eql(imp)
  end
end